#include <iostream>
#include <fstream>
#include<sstream>
#include <vector>
#include <string>
#include <map>
#include <conio.h>  // Include this for _getch() function
using namespace std;

// Define Question structure for quizzes
struct Question {
    string definition;
    vector<string> options;
    int correctOption;
};

// Define Course class
class Course {
public:
    string name;
    vector<string> chapters;
    map<string, vector<Question>> chapterQuizzes; // Chapter -> List of Questions
    map<string, string> chapterDefinitions; // Chapter -> Chapter Definition
};

// Define User class
class User {
public:
    string username;
    string password;
    vector<Course> enrolledCourses;
    int score;

    User(const string& u, const string& p) : username(u), password(p), score(0) {}
};

vector<User> users;
vector<Course> courses;
User* currentUser = nullptr;
bool loggedIn = false;

void saveUserDataToFile() {
    ofstream file("user_data.txt");
    for (const User& user : users) {
        file << user.username << " " << user.password << " " << user.score << endl;
    }
    file.close();
}

void loadUserDataFromFile() {
    ifstream file("user_data.txt");
    if (!file) {
        cout << "\t\t\t\t\t\tWelcome to the Learning App!" << endl;
        return;
    }
    while (!file.eof()) {
        string username, password;
        int score;
        file >> username >> password >> score;
        users.push_back(User(username, password));
        users.back().score = score;
    }
    file.close();
}

bool isUsernameUnique(const string& username) {
    for (const User& user : users) {
        if (user.username == username) {
            return false;
        }
    }
    return true;
}

bool isPasswordUnique(const string& password) {
    for (const User& user : users) {
        if (user.password == password) {
            return false;
        }
    }
    return true;
}

// Function to register a new user
void Register() {
    string username, password;
    cout << "Registration:\n";

    while (true) {
        cout << "Enter username: ";
        cin >> username;

        if (isUsernameUnique(username)) {
            break;
        } else {
            cout << "Username already exists. Please choose a different username.\n";
        }
    }

    while (true) {
        cout << "Enter password: ";
        char ch;
        password = "";
        while ((ch = _getch()) != '\r') {  // '\r' is the Enter key
            cout << '*';  // Display '*' for each character entered
            password += ch;
        }
        cout << endl;

        if (isPasswordUnique(password) && username != password) {
            break;
        } else if (!isPasswordUnique(password)) {
            cout << "Password already exists. Please choose a different password.\n";
        } else {
            cout << "Username and password cannot be the same. Please try again.\n";
        }
    }

    users.push_back(User(username, password));
    saveUserDataToFile();
    cout << "\n";
    cout << "Registration successful.\n";
}

User* loginUser() {
    string username, password;
    cout << "Login:\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    char ch;
    password = "";
    while ((ch = _getch()) != '\r') {
        cout << '*';  // Display '*' for each character entered
        password += ch;
    }
    cout << endl;

    for (User& user : users) {
        if (user.username == username && user.password == password) {
            cout << "\n";
            cout << "Login successful.\n";
            return &user;
        }
    }

    cout << "Login failed. Incorrect username or password.\n";
    return nullptr;
}

// Function to enroll in a course
void EnrollInCourse() {
    if (!loggedIn) {
        cout << "You need to be logged in to enroll in a course." << endl;
        return;
    }

    cout << "Available Courses:\n";
    for (int i = 0; i < courses.size(); ++i) {
        cout << i + 1 << ". " << courses[i].name << endl;
    }

    int choice;
    cout << "\n";
    cout << "Enter the course number to enroll: ";
    cin >> choice;

    if (choice >= 1 && choice <= courses.size()) {
        currentUser->enrolledCourses.push_back(courses[choice - 1]);
        cout << "Enrollment successful!\n";
    } else {
        cout << "Invalid course selection.\n";
    }
}

// Function to display a quiz
void displayQuiz(const vector<Question>& quiz) {
    for (int i = 0; i < quiz.size(); i++) {
        cout << "Question " << (i + 1) << ": " << quiz[i].definition << endl;
        for (const string& option : quiz[i].options) {
            cout << option << endl;
        }
    }
}

// Function to take a quiz
int takeQuiz(const vector<Question>& quiz) {
    int score = 0;
    for (int i = 0; i < quiz.size(); i++) {
        cout << "Enter your choice for question " << (i + 1) << " (1-" << quiz[i].options.size() << "): ";
        int userAnswer;
        cin >> userAnswer;
        if (userAnswer == quiz[i].correctOption) {
            cout << "Correct!\n";
            score++;
        } else {
            cout << "Incorrect. The correct answer is: " << quiz[i].options[quiz[i].correctOption - 1] << endl;
        }
    }
    currentUser->score += score;
    saveUserDataToFile();
    return score;
}

// Function to start a chapter
void StartChapter() {
    if (!loggedIn) {
        cout << "You need to be logged in to start a chapter." << endl;
        return;
    }
    
    if (currentUser->enrolledCourses.empty()) {
        cout << "You are not enrolled in any courses yet. Please enroll in a course first." << endl;
        return;
    }

    cout << "Enrolled Courses:\n";
    for (int i = 0; i < currentUser->enrolledCourses.size(); ++i) {
        cout << i + 1 << ". " << currentUser->enrolledCourses[i].name << endl;
    }

    int courseChoice;
    cout << "\n";
    cout << "Enter the course number to start learning: ";
    cin >> courseChoice;

    if (courseChoice >= 1 && courseChoice <= currentUser->enrolledCourses.size()) {
        Course& selectedCourse = currentUser->enrolledCourses[courseChoice - 1];
        cout << "Course: " << selectedCourse.name << endl;

        // Display chapters
        cout << "Chapters:\n";
        for (int i = 0; i < selectedCourse.chapters.size(); ++i) {
            cout << i + 1 << ". " << selectedCourse.chapters[i] << endl;
        }

        int chapterChoice;
        cout << "\n";
        cout << "Enter the chapter number to start: ";
        cin >> chapterChoice;

        if (chapterChoice >= 1 && chapterChoice <= selectedCourse.chapters.size()) {
            string selectedChapterName = selectedCourse.chapters[chapterChoice - 1];
            vector<Question>& selectedChapter = selectedCourse.chapterQuizzes[selectedChapterName];
            string chapterDefinition = selectedCourse.chapterDefinitions[selectedChapterName]; // Get the chapter definition

            cout << "Chapter: " << selectedChapterName << endl;

            // Display chapter definition
            cout << "\n";
            cout << "C++ is a general-purpose programming language and is widely used nowdays for competitive programming. " << chapterDefinition << endl;

            // Take quiz for this chapter
            cout << "\n";
            cout << "Starting quiz for this chapter:\n";
            displayQuiz(selectedChapter);
            int score = takeQuiz(selectedChapter);
            cout << "\n";
            cout << "Your score for this chapter: " << score << " out of " << selectedChapter.size() << endl;
        } else {
            cout << "Invalid chapter selection.\n";
        }
    } else {
        cout << "Invalid course selection.\n";
    }
}

int main() {
    loadUserDataFromFile();
    
    // Add courses details
    Course c1;
   c1.name = "C++ Programming";
   c1.chapters = { "Introduction to C++", "Variables and Data types in C++", "Input/Output in C++", "Operators in C++", "Conditional Statements and loops", "Functions in C++", "Pointers and memory allocation", "Arrays", "Strings", "Object-oriented programming (OOPS)", "Exception handling", "STL in C++"};
   courses.push_back(c1);

   // Define the quiz questions for "Introduction to C++"
   vector<Question> introToCppQuiz;
   Question q1;
   q1.definition = "What is C++?";
   q1.options = {"A programming language","A type of coffee","An animal"};
   q1.correctOption = 1;
   introToCppQuiz.push_back(q1);

   Question q2;
   q2.definition = "\nWhen was C++ first developed?";
   q2.options = {"1980","1995","2005"};
   q2.correctOption = 1;
   introToCppQuiz.push_back(q2);
   
   // Add the quiz questions to the corresponding chapter and set chapter definition
    courses[0].chapterQuizzes["Introduction to C++"] = introToCppQuiz;
    //courses[0].chapterDefinitions["Introduction to C++"] = "C++ is a general-purpose programming language and is widely used nowadays for competitive programming.";

    Course c2;
    c2.name = "Java Programming";
    c2.chapters = { "Introduction to Java", "Variables in Java"};
    courses.push_back(c2);
    
    while (true) {
        if (loggedIn) {
            cout << "\n";
            cout << "1. Enroll in a Course\n";
            cout << "2. Start Learning a Chapter\n";
            cout << "3. View Score\n";
            cout << "4. Logout\n";
            cout << "5. Quit\n";
            cout << "\n";
        } else {
            cout << "\n";
            cout << "1. Register\n";
            cout << "2. Login\n";
            cout << "3. Quit\n";
            cout << "\n";
        }

        int choice;
        cin >> choice;

        if (loggedIn) {
            switch (choice) {
                case 1:
                    EnrollInCourse();
                    break;
                case 2:
                    StartChapter();
                    break;
                case 3:
                    cout << "Your total score: " << currentUser->score << endl;
                    break;
                case 4:
                    loggedIn = false;
                    currentUser = nullptr;
                    break;
                case 5:
                    saveUserDataToFile();
                    return 0;
                default:
                    cout << "Invalid choice.\n";
            }
        } else {
            switch (choice) {
                case 1:
                    Register();
                    break;
                case 2:
                    currentUser = loginUser();
                    if (currentUser != nullptr) {
                        loggedIn = true;
                    }
                    break;
                case 3:
                    saveUserDataToFile();
                    return 0;
                default:
                    cout << "Invalid choice.\n";
            }
        }
    }

    return 0;
}