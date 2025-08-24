//============================================================================
// Name        : Project 2
// Author      : Logan McCullough 
// Version     : 1.0
//============================================================================

//added a few extra \n so that way i could see what was happening on the output for debugging purposes. 
//was having issues trimming leet code pointed to this 

//includes that will probably need more
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <limits>


#include <functional>
using namespace std;

//initial course structure following my bst tree suedough code 
struct Course
{
	string courseNumber;
	string courseName;
	vector<string> prereqs;

};


//class CourseBST

class CourseBST
{
private:
	struct Node
	{
		//course object
		Course course;  
		Node* left;
		Node* right;

		explicit Node(const Course& c) : course(c), left(nullptr), right(nullptr) {}
	};

	Node* root = nullptr;
	size_t count = 0;


	//left or right insert
	Node* insert(Node* node, const Course& c) 
	{
		if (node == nullptr) 
		{
			++count;
			return new Node(c);
		}


		if (c.courseNumber < node->course.courseNumber) 
		{
			node->left = insert(node->left, c);
		}
		else if (c.courseNumber > node->course.courseNumber) 
		{
			node->right = insert(node->right, c);
		}
		else 
		{
			//if duplicate replace
			node->course = c;
		}
		return node;
	}

	//check all
	const Course* search(Node* node, const string& key) const
	{
		while (node != nullptr) 
		{
			if (key == node->course.courseNumber) 
			{
				return &node->course;
			}
			else if (key < node->course.courseNumber) 
			{
				node = node->left;
			}
			else 
			{
				node = node->right;
			}
		}
		//if not found
		return nullptr;
	}

	//mem leak fixer
	void destroy(Node* node) 
	{
		if (node == nullptr) return;
		destroy(node->left);
		destroy(node->right);
		delete node;
	}

	//normal indorder with pointer
	void inorder(Node* node, void (*visit)(const Course&)) const
	{
		if (node == nullptr) return;
		inorder(node->left, visit);
		visit(node->course);
		inorder(node->right, visit);
	}

public:

	CourseBST() = default;

	//cleanup destructor
	~CourseBST()
	{
		destroy(root);
	}

	CourseBST(const CourseBST&) = delete;
	CourseBST& operator=(const CourseBST&) = delete;

	void clear() {                         
		destroy(root);
		root = nullptr;
		count = 0;
	}

	//insert a course
	void insert(const Course& c) { root = insert(root, c);}

	//search exact course number
	const Course* search(const string& key) const { return search(root, key);}

	//in order traverse
	void inorder(void (*visit)(const Course&)) const { inorder(root, visit);}

};


//global
static CourseBST gCourseTree;


//print prereqs if any
static void printCourseDetail(const Course& c)
{
	std::cout << c.courseNumber << ", " << c.courseName << "\n";
	if (c.prereqs.empty())
	{
		//print none if none 
		std::cout << "Prerequisites: None\n";
	}
	else
	{
		//print out prereqs in order
		std::cout << "Prerequisites: ";
		for (size_t i = 0; i < c.prereqs.size(); ++i) 
		{
			std::cout << c.prereqs[i];
			if (i + 1 < c.prereqs.size()) std::cout << ", ";
		}
		
	}
}



//////////////////////////////////////////////////splitting it up maybe we see how this goes 
//testing is space and subs tring cutting 
static inline std::string trim(const std::string& s) 
{
	size_t i = 0, j = s.size();
	while (i < j && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
	while (j > i && std::isspace(static_cast<unsigned char>(s[j - 1]))) --j;
	return s.substr(i, j - i);
}
///////////////////////////////////////////////////////////end testing spot 



//print course line 
static void printCourseLine(const Course& c) 
{
	cout << c.courseNumber << ", " << c.courseName << " \n";
}

bool loadCoursesFromCSV(std::vector<Course>& outCourses) 
{
	std::ifstream file("CS 300 ABCU_Advising_Program_Input.csv");
	
	//debug error prompting 
	if (!file.is_open()) 
	{
		std::cerr << "Error: could not open 'CS 300 ABCU_Advising_Program_Input.csv'\n\n";
		return false;
	}


	outCourses.clear();
	std::string line;

	while (std::getline(file, line)) 
	{
		if (line.empty()) continue;

		std::stringstream ss(line);
		std::string field;

		Course course;

		// coursenumber
		if (!std::getline(ss, field, ',')) continue;
		course.courseNumber = trim(field);

		// courseName
		if (!std::getline(ss, field, ',')) field.clear();
		course.courseName = trim(field);

		// prerequisites 
		while (std::getline(ss, field, ',')) 
		{
			field = trim(field);
			if (!field.empty()) course.prereqs.push_back(field);
		}

		outCourses.push_back(std::move(course));
	}

	return true;
}

void printCourses(const std::vector<Course>& courses) 
{
	if (courses.empty()) 
	{
		std::cout << "(No courses loaded)\n";
		return;
	}

	for (const auto& c : courses) 
	{
		std::cout << c.courseNumber << " - " << c.courseName;
		if (!c.prereqs.empty()) 
		{
			std::cout << " | Prereqs: ";
			for (size_t i = 0; i < c.prereqs.size(); ++i) 
			{
				std::cout << c.prereqs[i];
				if (i + 1 < c.prereqs.size()) std::cout << ", ";
			}
		}
		std::cout << "\n";
	}
}



//////////////////////////test main to make sure compile errors 
int main() 
{
	bool isRunning = true;
	bool isDataLoaded = false;
	bool validFormat = true;
	std::vector<Course> courses;  
	   
		while (isRunning) {
			std::cout << "\nMenu:\n";
			std::cout << "1. Load Data\n";
			std::cout << "2. Print Courses\n";
			std::cout << "3. Print Course Details\n";
			std::cout << "9. Exit\n";
			std::cout << "Enter choice: ";

			int choice;
			if (!(std::cin >> choice)) 
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input, numbers only.\n";
			}

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

			if (choice == 1) 
			{
				
				//node clear 
				validFormat = true;
				if (loadCoursesFromCSV(courses)) 
				{
					gCourseTree.clear();                 
					for (const auto& c : courses) 
					{
						gCourseTree.insert(c);
					}
					isDataLoaded = true;
					std::cout << "\nLOADED\n";
				}
				else 
				{
					validFormat = false;
					std::cout << "Error loading file.\n\n";
				}
			}
			else if (choice == 2) 
			{
				//validation check
				if (!isDataLoaded) 
				{
					std::cout << "Choose 1 first.\n";
				}
				else 
				{
					//print
					std::cout << "\nCourses (sorted):\n\n";
					gCourseTree.inorder(&printCourseLine);
					std::cout << "\n";
				}
			}
			else if (choice == 3) 
			{
				if (!isDataLoaded) 
				{
					//validation check
					std::cout << "Choose 1 first.\n";
				}
				else 
				{
					//case sensitive so be warned 
					std::string cnum;
					std::cout << "\nEnter Course num: ";
					std::getline(std::cin, cnum);

					const Course* found = gCourseTree.search(cnum);
					if (found) 
					{
						printCourseDetail(*found);
						std::cout << "\n";
					}
					else 
					{
						std::cout << "\nCourse not found.\n";
					}
				}
			}
			//changing spacing
			else if (choice == 9) 
			{
				std::cout << "\nThank you for using the course planner!\n";
				isRunning = false;
			}
			else {
				std::cout << "Choice must be 1, 2, 3, or 9 only.\n";
			}
		}

		return 0;
	}
