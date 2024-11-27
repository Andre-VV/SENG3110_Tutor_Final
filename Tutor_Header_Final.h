#ifndef TUTOR_MAIN_FINAL_H
#define TUTOR_MAIN_FINAL_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <math.h>
#include <list>
#include <sstream>


using namespace std;

// File names to use in functions
extern string filename1; // ListOfTutors_F.txt
extern string filename2; // KeyList_F.txt

// Function Declarations

// Adds a tutor to both the main data file and the key file
void add_tutor(const string& fname, const string& lname, const string& subject,
    const string& price, const string& city, const string& country,
    const string& email, const string& bio);

// Converts a string from a file to a vector using "|" as a delimiter
vector<string> line_to_vector(const string& line);

list<vector<string>> listSort(list<vector<string>> input_list, vector<string> input_vec);

queue<vector<string>> sortQ(queue<vector<string>> input);

string joinTutorLine(const vector<string>& tutorDetails);
// Searches the key file for a key and adds it to a queue
queue<vector<string>> SearchKeyFile(queue<vector<string>> Results, string key1);

// Prints a vector to the terminal
void printV(vector<string> input);

// Prints a queue of vectors to the terminal
void printQ(queue<vector<string>> input);

// Gets a tutor's line from the main data file based on the location
vector<string> GetTutorLine(double location);

// Gets the rating or the number of ratings from a line in the main data file
double getRating(double location, bool R_or_num);

// Inserts a rating and number of ratings into the main data file and updates the key file
void insert_New_Rating(double location, double newRating);

// Rates a tutor by calculating the new rating and number of ratings, and then updates the files
void Rate_Tutor(double location, double new_rate);

#endif 
