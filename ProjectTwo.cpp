//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Joseph Malecha
// Description : CS300 Project Two
//============================================================================

#include <iostream>
#include <fstream> //ifstream, open and read CSV file
#include <sstream> //stringstream, makes a line readable as a stream
#include <string> 
#include <vector>
#include <algorithm> //sort function
#include <cctype> // toupper()

using namespace std;

//struct to store information for a single course
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

class HashTable {
    private:

        //bucket contains linked list of nodes, node holds Course object and pointer to next node
        struct Node {
            Course course;
            Node* next;

            Node(const Course& course) {
                this->course = course;
                this->next = nullptr;
            }
        };

        //number of buckets in the hashtable, fixed amount
        static const int BUCKET_COUNT = 101;
        vector<Node*> buckets;

        //calculate bucket placement by courseNumber
        int hash(const string& courseNumber) const {
            unsigned int hashValue = 0;

            for (int i = 0; i < courseNumber.length(); i++) { 
                hashValue = hashValue * 31 + courseNumber[i];
            }

            return hashValue % BUCKET_COUNT;
        }

    public:
        //initialize buckets as empty
        HashTable() {
            buckets.resize(BUCKET_COUNT, nullptr);
        }

        //free allocated memory when hashtable is destroyed
        ~HashTable() {
            clear();
        }

        //remove courses from hashtable
        void clear() {
            for (int i = 0; i < BUCKET_COUNT; i++) {
                Node* current = buckets[i];

                while (current != nullptr) {
                    Node* temp = current;
                    current = current->next;
                    delete temp;
                }

                buckets[i] = nullptr;
            }
        }

        //insert course at start of bucket's linked list
        void insert(const Course& course) {
            int bucketIndex = hash(course.courseNumber);

            Node* newNode = new Node(course);
            newNode->next = buckets[bucketIndex];
            buckets[bucketIndex] = newNode;
        }

        //search hashtable for course by courseNumber
        const Course* findCourse(const string& courseNumber) const {
            int bucketIndex = hash(courseNumber); //pass to hash function to find bucket the course will be in
            Node* current = buckets[bucketIndex]; //initialize pointer to first node in bucket

            //search the linked list until the course is found or reach end
            while (current != nullptr) {
                if (current->course.courseNumber == courseNumber) {
                    return &current->course; //return address of course object found
                }

                current = current->next; //go to next node
            }

            return nullptr; //if no match found
        }

        //UPDATED: remove course from hashtable
        bool removeCourse(const string& courseNumber) {
            int bucketIndex = hash(courseNumber);
            Node* current = buckets[bucketIndex];
            Node* previous = nullptr;

            while (current != nullptr) {
                if (current->course.courseNumber == courseNumber) {
                    if (previous == nullptr) {
                        buckets[bucketIndex] = current->next;
                    }
                    else {
                        previous->next = current->next;
                    }

                    delete current;
                    return true;
                }

                previous = current;
                current = current->next;
            }

            return false;
        }

        //return all courses stored in the hashtable
        vector<Course> getAllCourses() const {
            vector<Course> allCourses;

            //iterate through each bucket
            for (int i = 0; i < BUCKET_COUNT; i++) {
                Node* current = buckets[i];

                //go through each node in the bucket's linked list
                while (current != nullptr) {
                    allCourses.push_back(current->course);
                    current = current->next;
                }
            }

            return allCourses;
        }
};

bool validatePrerequisites(HashTable& courses); //forward declaration of validatePrerequisites

string trim(const string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");

    if (start == string::npos) {
        return "";
    }

    size_t end = text.find_last_not_of(" \t\r\n");

    return text.substr(start, end - start + 1);
}

//UPDATED: convert letters in string to uppercase
string toUpperCase(string text) {
    for (int i = 0; i < text.length(); i++) {
        text[i] = toupper(static_cast<unsigned char>(text[i]));
    }

    return text;
}

//loadCourses function, read course info from CSV file into the hashtable. UPDATED to skip empty course
bool loadCourses(const string& filename, HashTable& courses) {
    ifstream file(filename);

    //open check
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return false;
    }

    string line;

    //read each line of the CSV file
    while (getline(file, line)) {
        stringstream lineStream(line); //makes the line readable as a stream 
        string parameter;
        vector<string> parameters;

        //separate the line into fields using commas 
        while (getline(lineStream, parameter, ',')) { 
            parameters.push_back(trim(parameter)); //UPDATED: trim()
        }

        //UPDATED: check each course has at least a course number and title, continue
        if (parameters.size() < 2) {
            cout << "Warning: Skipping invalid course data." << endl;
            continue;
        }

        //check course number and title are not empty
        if (parameters[0].empty() || parameters[1].empty()) {
            cout << "Warning: Skipping course with missing number or title." << endl;
            continue;
        }

        //create course object using the parameters
        Course course;
        course.courseNumber = parameters[0];
        course.courseTitle = parameters[1];

        //store additional parameters as prerequisite course numbers
        for (int i = 2; i < parameters.size(); i++) {
            if (!parameters[i].empty()) {
                course.prerequisites.push_back(parameters[i]);
            }
        }

        //check if duplicate course
        if (courses.findCourse(course.courseNumber) != nullptr) {
            cout << "Warning: Skipping duplicate course " << course.courseNumber << endl;
            continue;
        }

        //insert completed course object into the hash table
        courses.insert(course);
    }

    //check if file empty
    if (courses.getAllCourses().empty()) {
        cout << "Error: No courses found." << endl;
        file.close();
        return false;
    }
    
    file.close();

    if (!validatePrerequisites(courses)) {
        return false;
    }

    return true;
}


//UPDATED whole function to work with removeCourse()
bool validatePrerequisites(HashTable& courses) { //forward declaration of validatePrerequisites
    bool courseRemoved;

    do {
        courseRemoved = false;
        vector<Course> allCourses = courses.getAllCourses();

        //check each course
        for (int i = 0; i < allCourses.size(); i++) {

            //check each prereq
            for (int j = 0; j < allCourses[i].prerequisites.size(); j++) {
                string prerequisite = allCourses[i].prerequisites[j];

                if (courses.findCourse(prerequisite) == nullptr) {
                    cout << "Warning: Skipping course " << allCourses[i].courseNumber << " due to invalid prerequisite " << prerequisite << endl;

                    courses.removeCourse(allCourses[i].courseNumber);
                    courseRemoved = true;
                    break;
                }
            }
        }
    }

    while (courseRemoved);
    return !courses.getAllCourses().empty();
}

//compare two courses by their course number
bool compareCourses(const Course& a, const Course& b) {
    return a.courseNumber < b.courseNumber;
}

//get and sort all courses by course number
vector<Course> sortCourses(const HashTable& courses) {
    vector<Course> sortedCourses = courses.getAllCourses();

    //pass to sort using beginning of vector, end of vector, above compareCourses function
    sort(sortedCourses.begin(), sortedCourses.end(), compareCourses);

    return sortedCourses;
}

//print courses in ascending alphanumeric order
void printCourseList(const HashTable& courses) {
    vector<Course> sortedCourses = sortCourses(courses);

    //iterate through and print each course number and title
    for (int i = 0; i < sortedCourses.size(); i++) {
        cout << sortedCourses[i].courseNumber << ", " << sortedCourses[i].courseTitle << endl;
    }
}

//search and print a course and it's prereqs
void searchCourse(const HashTable& courses, const string& courseNumber) {
    const Course* course = courses.findCourse(courseNumber);

    //check if found from findCourse function
    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    //print coursenumber and coursetitle
    cout << course->courseNumber << ", " << course->courseTitle << endl;

    //print each prereq coursenumber and coursetitle
    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites:" << endl;

        //iterate through and print each prereq
        for (int i = 0; i < course->prerequisites.size(); i++) {
            const Course* prerequisite = courses.findCourse(course->prerequisites[i]);

            if (prerequisite != nullptr) {
                cout << prerequisite->courseNumber << ", " << prerequisite->courseTitle << endl;
            }
        }
    }
}

//main (menu)
int main() {
    HashTable courses;
    bool dataLoaded = false;
    int choice = 0;
    string filename;
    string courseNumber;

    //Display menu until option 9 is selected
    while (choice != 9) {
        cout << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        //input validation for menu options
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n'); //discard up to 1000 characters, or until the end of the line

            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        //discard extra characters in input (i.e. 34xyz inputed produces 34 as an invalid menu option, discards xyz)
        cin.ignore(1000, '\n'); 

        if (choice == 1) {
            cout << "Enter file name: ";
            getline(cin, filename); //rather than "cin >> filename" in case the filename has any whitespace

            //clear existing data before loading new data (file)
            courses.clear();
            dataLoaded = false;

            dataLoaded = loadCourses(filename, courses); //pass file to loadCourses

            if (dataLoaded) {
                cout << "Course data loaded successfully." << endl;
            }
            else {
                courses.clear();
            }
        }
        else if (choice == 2) {
            if (dataLoaded) {
                printCourseList(courses);
            }
            else {
                cout << "Please load course data first." << endl;
            }
        }
        else if (choice == 3) {
            if (dataLoaded) {
                cout << "Enter course number: ";
                cin >> courseNumber;

                //remove whitespace and conveert string to uppercase before searching
                courseNumber = toUpperCase(trim(courseNumber));

                searchCourse(courses, courseNumber);
            }
            else {
                cout << "Please load course data first." << endl;
            }
        }
        else if (choice == 9) {
            cout << "Goodbye." << endl;
        }
        else {
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}
