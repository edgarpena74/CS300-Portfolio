/*
 * Project 2
 *
 * Edgar Ignacio Pena
 * CS-300
 * June 26, 2025
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;

// Struct to hold course data
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Compare two courses by their number for sorting
bool compareCourses(const Course &a, const Course &b) {
    return a.courseNumber < b.courseNumber;
}

// In-memory list of all courses
static vector<Course> courses;

// Check that a course code is 4 letters followed by digits (e.g., CSCI200)
bool isValidCourseNumber(const string& courseNum) {
    if (courseNum.size() < 5) return false;
    // first 4 must be letters
    for (int i = 0; i < 4; ++i) {
        if (!isalpha(courseNum[i])) return false;
    }
    // remaining must be digits
    for (size_t i = 4; i < courseNum.size(); ++i) {
        if (!isdigit(courseNum[i])) return false;
    }
    return true;
}

// Allow only "1", "2", "3" or "9" at the main menu
bool isValidMenuChoice(const string& choice) {
    return choice == "1" || choice == "2" || choice == "3" || choice == "9";
}

// Load data from CSV into the courses vector
void loadData() {
    courses.clear();  // start fresh

    cout << "\nEnter course data file name: ";
    string fname;
    getline(cin, fname);

    ifstream in(fname.c_str());
    if (!in) {
        cerr << "Error: Could not open file \"" << fname << "\"\n";
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;  // skip blanks

        stringstream ss(line);
        Course c;
        // parse course number and name
        if (!getline(ss, c.courseNumber, ',')) continue;
        if (!getline(ss, c.courseName, ',')) continue;

        // validate course number
        if (!isValidCourseNumber(c.courseNumber)) {
            cerr << "Invalid course number -> " << c.courseNumber << "\n";
            continue;
        }

        // skip duplicates
        bool dup = false;
        for (size_t i = 0; i < courses.size(); ++i) {
            if (courses[i].courseNumber == c.courseNumber) {
                dup = true;
                break;
            }
        }
        if (dup) {
            cerr << "Duplicate course: " << c.courseNumber << " — skipping\n";
            continue;
        }

        // read prerequisites
        string pre;
        while (getline(ss, pre, ',')) {
            if (!pre.empty())
                c.prerequisites.push_back(pre);
        }
        // sort prereqs for consistent output
        sort(c.prerequisites.begin(), c.prerequisites.end());

        courses.push_back(c);
    }

    in.close();
    cout << "\nCourse data successfully loaded into memory.\n";
}

// Print all courses in alphanumeric order
void printCourseList() {
    if (courses.empty()) {
        cout << "Error: No course data loaded.\n";
        return;
    }

    sort(courses.begin(), courses.end(), compareCourses);

    cout << "\nHere is a sample schedule:\n\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        cout << courses[i].courseNumber << ": "
             << courses[i].courseName << "\n\n";
    }
}

// Build the prerequisites line as a single string
/*string formatPrerequisites(const Course &c) {
    string out = "Prerequisites: ";
    if (c.prerequisites.empty()) {
        out += "None";
    } else {
        for (size_t i = 0; i < c.prerequisites.size(); ++i) {
            out += c.prerequisites[i];
            if (i + 1 < c.prerequisites.size())
                out += " ";
        }
    }
    return out;
}*/

string formatPrerequisites(const Course &c) {
    if (c.prerequisites.empty()) {
        return "None";
    }

    string out = "Prerequisites: " + c.prerequisites[0];
    for (size_t i = 1; i < c.prerequisites.size(); ++i) {
        out += " " + c.prerequisites[i];
    }
    return out;
}

// Show all details for one course
void showCourseDetails(const Course &c) {
    cout << "\n\n" << c.courseNumber << ": " << c.courseName << "";

     cout << "\n\n" << formatPrerequisites(c) << "\n\n";
}

// Prompt for a single course and display its details
void printCourseInfo() {
    while (true) {
        cout << "\nEnter the course using the format AAAA123: ";
        string input;
        getline(cin, input);
        // normalize to uppercase
        for (size_t i = 0; i < input.size(); ++i)
            input[i] = toupper(input[i]);

        if (!isValidCourseNumber(input)) {
            cout << "Invalid Entry.\n";
            continue;
        }

        // lookup course
        Course *found = NULL;
        for (size_t i = 0; i < courses.size(); ++i) {
            if (courses[i].courseNumber == input) {
                found = &courses[i];
                break;
            }
        }
        if (!found) {
            cout << "Invalid Entry.\n";
            continue;
        }

        // display via helper
        showCourseDetails(*found);
        break;
    }
}

// Show the main menu
void displayMenu() {
    cout << "1. Load Data Structure.\n"
         << "2. Print Course List.\n"
         << "3. Print Course.\n"
         << "9. Exit\n\n";
}

// Main loop: handle menu and direct code entry
void runCoursePlanner() {
    string choice;
    while (true) {
        displayMenu();
        cout << "What would you like to do? ";
        getline(cin, choice);

        // direct course-code shortcut
        if (!isValidMenuChoice(choice)) {
            // normalize to uppercase
            for (size_t i = 0; i < choice.size(); ++i)
                choice[i] = toupper(choice[i]);
            if (isValidCourseNumber(choice)) {
                // lookup
                Course *found = NULL;
                for (size_t j = 0; j < courses.size(); ++j) {
                    if (courses[j].courseNumber == choice) {
                        found = &courses[j];
                        break;
                    }
                }
                if (found) {
                    showCourseDetails(*found);
                    continue;
                }
            }
            cout << "\nError -> Please input a valid number choice.\n\n";
            continue;
        }

        // menu options
        if (choice == "1") {
            loadData();
        } else if (choice == "2") {
            printCourseList();
        } else if (choice == "3") {
            printCourseInfo();
        } else {  // "9"
            cout << "\nThank you for using the course planner!\n";
            break;
        }

        cout << endl;
    }
}

int main() {
    runCoursePlanner();
    return 0;
}