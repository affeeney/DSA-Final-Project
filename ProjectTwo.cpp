#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};
struct TreeNode {
    Course course;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Course c) : course(c), left(nullptr), right(nullptr) {}
};
class BinaryTree {
public:
    BinaryTree() : root(nullptr) {}

    void insert(const Course& course) {
        root = insertNode(root, course);
    }
    void inOrderTraversal() const {
        inOrderTraversal(root);
    }
    const Course* search(const string& courseNumber) const {
        return searchNode(root, courseNumber);
    }

private:
    TreeNode* root;

    TreeNode* insertNode(TreeNode* node, const Course& course) {
        if (!node) return new TreeNode(course);
        if (course.courseNumber < node->course.courseNumber) {
            node->left = insertNode(node->left, course);
        }
        else {
            node->right = insertNode(node->right, course);
        }
        return node;
    }

    void inOrderTraversal(TreeNode* node) const {
        if (!node) return;
        inOrderTraversal(node->left);
        if (isCSOrMathCourse(node->course)) {
            cout << node->course.courseNumber << ": " << node->course.title << endl;
        }
        inOrderTraversal(node->right);
    }

    const Course* searchNode(TreeNode* node, const string& courseNumber) const {
        if (!node) return nullptr;
        if (courseNumber == node->course.courseNumber) {
            return &node->course;
        } else if (courseNumber < node->course.courseNumber) {
            return searchNode(node->left, courseNumber);
        } 
        else {
            return searchNode(node->right, courseNumber);
        }
    }

    bool isCSOrMathCourse(const Course& course) const {
        return course.title.find("Computer Science") != string::npos || course.title.find("Math") != string::npos;
    }
};

vector<Course> loadCourses(const string& fileName) {
    vector<Course> courses;
    ifstream file(fileName);
    string line;
    if (!file.is_open()) {
        cout << "error" << endl;
        return courses;
    }
    while (getline(file, line)) {
        stringstream ss(line);
        Course course;
        string prereq;
        getline(ss, course.courseNumber, ',');
        getline(ss, course.title, ',');
        while (getline(ss, prereq, ',')) {
            course.prerequisites.push_back(prereq);
        }
        courses.push_back(course);
    }

    file.close();
    return courses;
}

void printCourseInfo(const BinaryTree& tree, const string& courseNumber) {
    const Course* course = tree.search(courseNumber);
    if (course) {
        cout << course->courseNumber << ": " << course->title << endl;
        cout << "prereqs: ";
        if (course->prerequisites.empty()) {
            cout << "null" << endl;
        } 
        else {
            for (const auto& prereq : course->prerequisites) {
                cout << prereq << " ";
            }
            cout << endl;
        }
    } else {
        cout << "error." << endl;
    }
}

int main() {
    BinaryTree courseTree;
    string fileName;
    int choice = 0;
    while (choice != 9) {
        cout << endl;
        cout << "----------------------------------------" << endl;
        cout << "menu" << endl;
        cout << "1. course data" << endl;
        cout << "2. print courses" << endl;
        cout << "3. get info" << endl;
        cout << "9. exit" << endl;
        cout << "choose an option: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "enter the filename ";
                cin >> fileName;
                for (const auto& course : loadCourses(fileName)) {
                    courseTree.insert(course);
                }
                cout << "loaded!" << endl;
                break;
            case 2:
                cout << "heres a sorted list of all your cs and math courses:" << endl;
                courseTree.inOrderTraversal();
                break;
            case 3: {
                string courseNumber;
                cout << "coursenumber? : ";
                cin >> courseNumber;
                printCourseInfo(courseTree, courseNumber);
                break;
            }
            case 9:
                cout << "exit!" << endl;
                break;
            default:
                cout << "error" << endl;
                break;
        }
    }

    return 0;
}

