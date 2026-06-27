//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Jeff Roys
// Version     : 1.0
// Copyright   : Copyright (c) 2026 SNHU COCE
// Description : CS 300 Project Two ABCU Advising Assistance Program
//============================================================================

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//============================================================================
// Global definitions visible to all methods
//============================================================================

// define a structure to hold course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// forward declarations
string trim(string text);
string toUpperCase(string text);
bool compareCourses(Course courseOne, Course courseTwo);
int findCourse(vector<Course>& courses, string courseNumber);
void loadCourses(string fileName, vector<Course>& courses);
void printCourseList(vector<Course> courses);
void printCourse(vector<Course>& courses);
void printMenu();

//============================================================================
// Course helper methods
//============================================================================

/**
 * Remove extra spaces from the beginning and end of a string.
 *
 * @param text The string to clean up
 * @return The same string without extra spaces
 */
string trim(string text) {
    while (!text.empty() && isspace(text.front())) {
        text.erase(text.begin());
    }

    while (!text.empty() && isspace(text.back())) {
        text.pop_back();
    }

    return text;
}

/**
 * Convert a string to uppercase.
 * This lets the user type csci400 and still find CSCI400.
 *
 * @param text The string to convert
 * @return The uppercase version of the string
 */
string toUpperCase(string text) {
    for (unsigned int i = 0; i < text.length(); ++i) {
        text[i] = toupper(text[i]);
    }

    return text;
}

/**
 * Compare two courses by course number.
 * This is used by sort() to print courses in alphanumeric order.
 *
 * @param courseOne First course to compare
 * @param courseTwo Second course to compare
 * @return true if courseOne should come before courseTwo
 */
bool compareCourses(Course courseOne, Course courseTwo) {
    return courseOne.courseNumber < courseTwo.courseNumber;
}

/**
 * Find a course in the vector by its course number.
 *
 * @param courses Vector containing all loaded courses
 * @param courseNumber The course number to search for
 * @return The index of the course, or -1 if not found
 */
int findCourse(vector<Course>& courses, string courseNumber) {
    courseNumber = toUpperCase(trim(courseNumber));

    for (unsigned int i = 0; i < courses.size(); ++i) {
        if (courses[i].courseNumber == courseNumber) {
            return i;
        }
    }

    return -1;
}

/**
 * Load course data from a CSV file into a vector.
 * Each line should contain course number, course title, and optional prerequisites.
 *
 * @param fileName The file path to load
 * @param courses Vector to store course objects
 */
void loadCourses(string fileName, vector<Course>& courses) {
    ifstream inputFile(fileName);
    string line;

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file " << fileName << endl;
        return;
    }

    courses.clear();

    while (getline(inputFile, line)) {
        if (trim(line).empty()) {
            continue;
        }

        stringstream lineStream(line);
        string field;
        vector<string> fields;

        while (getline(lineStream, field, ',')) {
            fields.push_back(trim(field));
        }

        if (fields.size() < 2) {
            cout << "Error: Invalid course line skipped." << endl;
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(fields[0]);
        course.courseTitle = fields[1];

        for (unsigned int i = 2; i < fields.size(); ++i) {
            if (!fields[i].empty()) {
                course.prerequisites.push_back(toUpperCase(fields[i]));
            }
        }

        courses.push_back(course);
    }

    inputFile.close();

    // Check that each listed prerequisite is also in the course list.
    for (unsigned int i = 0; i < courses.size(); ++i) {
        for (unsigned int j = 0; j < courses[i].prerequisites.size(); ++j) {
            if (findCourse(courses, courses[i].prerequisites[j]) == -1) {
                cout << "Error: Prerequisite " << courses[i].prerequisites[j]
                     << " for " << courses[i].courseNumber
                     << " was not found in the file." << endl;
            }
        }
    }

    cout << courses.size() << " courses loaded successfully." << endl;
}

/**
 * Print all courses in alphanumeric order.
 *
 * @param courses Vector containing all loaded courses
 */
void printCourseList(vector<Course> courses) {
    if (courses.empty()) {
        cout << "No course data is loaded. Please load the file first." << endl;
        return;
    }

    sort(courses.begin(), courses.end(), compareCourses);

    cout << "Here is a sample schedule:" << endl;

    for (unsigned int i = 0; i < courses.size(); ++i) {
        cout << courses[i].courseNumber << ", " << courses[i].courseTitle << endl;
    }
}

/**
 * Print one course and its prerequisites.
 *
 * @param courses Vector containing all loaded courses
 */
void printCourse(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No course data is loaded. Please load the file first." << endl;
        return;
    }

    string courseNumber;

    cout << "What course do you want to know about? ";
    getline(cin, courseNumber);

    int courseIndex = findCourse(courses, courseNumber);

    if (courseIndex == -1) {
        cout << "Course " << toUpperCase(trim(courseNumber)) << " not found." << endl;
        return;
    }

    Course course = courses[courseIndex];

    cout << course.courseNumber << ", " << course.courseTitle << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";

        for (unsigned int i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

/**
 * Display the main menu to the user.
 */
void printMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

//============================================================================
// Main method
//============================================================================

/**
 * The one and only main() method
 */
int main() {
    vector<Course> courses;
    string fileName;
    string choice;

    cout << "Welcome to the course planner." << endl;

    while (choice != "9") {
        printMenu();
        getline(cin, choice);
        choice = trim(choice);

        if (choice == "1") {
            cout << "Enter the course file name: ";
            getline(cin, fileName);
            fileName = trim(fileName);

            if (fileName.empty()) {
                cout << "Error: File name cannot be empty." << endl;
            }
            else {
                loadCourses(fileName, courses);
            }
        }
        else if (choice == "2") {
            printCourseList(courses);
        }
        else if (choice == "3") {
            printCourse(courses);
        }
        else if (choice == "9") {
            cout << "Thank you for using the course planner!" << endl;
        }
        else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}