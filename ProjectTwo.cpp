// Federico Gabaldon
// CS 300 - Project Two
// AI Use Acknowledgment: I used ChatGPT (OpenAI) to help explain C++ concepts,
// review program logic, and assist with testing and debugging.
// I reviewed, tested, and understand the final code.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <limits>

using namespace std;

// Structure to hold information for one course
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};
// Node used by the binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Binary search tree used to store courses
class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void deleteTree(Node* node);

public:
    BinarySearchTree();
    ~BinarySearchTree();

    void Insert(Course course);
    Course Search(string courseNumber);
    void InOrder();
    void Clear();
};

// Constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
    deleteTree(root);
}

// Recursively delete all nodes in the tree
void BinarySearchTree::deleteTree(Node* node) {
    if (node != nullptr) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

// Insert a course into the binary search tree
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

// Add a course node in the correct position
void BinarySearchTree::addNode(Node* node, Course course) {

    if (course.courseNumber < node->course.courseNumber) {

        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {

        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

// Search for a course by course number
Course BinarySearchTree::Search(string courseNumber) {

    Node* current = root;

    while (current != nullptr) {

        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }
        else if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return Course();
}

// Print all courses in alphanumeric order
void BinarySearchTree::InOrder() {
    inOrder(root);
}

// Recursively print the tree in order
void BinarySearchTree::inOrder(Node* node) {

    if (node != nullptr) {
        inOrder(node->left);

        cout << node->course.courseNumber << ", "
             << node->course.courseTitle << endl;

        inOrder(node->right);
    }
}

// Clear all nodes from the tree
void BinarySearchTree::Clear() {
    deleteTree(root);
    root = nullptr;
}

// Remove extra spaces from the beginning and end of a string
string trim(string text) {

    size_t first = text.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");

    return text.substr(first, last - first + 1);
}

// Convert a string to uppercase
string toUpperCase(string text) {

    for (char& character : text) {
        character = toupper(character);
    }

    return text;
}

// Load course data from the CSV file into the binary search tree
bool loadCourses(string fileName, BinarySearchTree& courseTree) {

    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: file could not be opened." << endl;
        return false;
    }

    courseTree.Clear();

    string line;

    while (getline(file, line)) {

        if (trim(line).empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        Course course;

        // Read course number
        getline(ss, token, ',');
        course.courseNumber = trim(token);

        // Read course title
        getline(ss, token, ',');
        course.courseTitle = trim(token);

        // Read any prerequisites
        while (getline(ss, token, ',')) {
            token = trim(token);

            if (!token.empty()) {
                course.prerequisites.push_back(token);
            }
        }

        // Make sure the course has a number and title
        if (course.courseNumber.empty() || course.courseTitle.empty()) {
            cout << "Error: invalid course data." << endl;
            courseTree.Clear();
            file.close();
            return false;
        }

        courseTree.Insert(course);
    }

    file.close();
    return true;
}

// Print information for one course
void printCourseInformation(BinarySearchTree& courseTree, string courseNumber) {

    courseNumber = toUpperCase(trim(courseNumber));

    Course course = courseTree.Search(courseNumber);

    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", "
         << course.courseTitle << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites:" << endl;

        for (string prerequisiteNumber : course.prerequisites) {

            Course prerequisiteCourse =
                    courseTree.Search(prerequisiteNumber);

            if (!prerequisiteCourse.courseNumber.empty()) {
                cout << prerequisiteCourse.courseNumber << ", "
                     << prerequisiteCourse.courseTitle << endl;
            }
        }
    }
}

int main() {

    BinarySearchTree courseTree;
    int choice = 0;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    do {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;

        cout << endl;
        cout << "What would you like to do? ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
        }

        if (choice == 1) {

            string fileName;

            cout << "Enter the course data file name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, fileName);

            dataLoaded = loadCourses(fileName, courseTree);

            if (dataLoaded) {
                cout << "Course data loaded successfully." << endl;
            }
        }

        else if (choice == 2) {

            if (!dataLoaded) {
                cout << "Load a valid file first." << endl;
            }
            else {
                cout << "Here is a sample schedule:" << endl;
                cout << endl;

                courseTree.InOrder();
            }
        }

        else if (choice == 3) {

            if (!dataLoaded) {
                cout << "Load a valid file first." << endl;
            }
            else {

                string courseNumber;

                cout << "What course do you want to know about? ";
                cin >> courseNumber;

                printCourseInformation(courseTree, courseNumber);
            }
        }

        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }

        else {
            cout << choice << " is not a valid option." << endl;
        }


    } while (choice != 9);

    return 0;
}


