#ifndef TUTOR_MAIN_FINAL_H
#define TUTOR_MAIN_FINAL_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <cmath>
#include <list>
#include <sstream>
#include <algorithm>
#include <functional>

// File names to use in functions
extern std::string filename1; // ListOfTutors_F.txt
extern std::string filename2; // KeyList_F.txt

// Function Declarations

// Adds a tutor to both the main data file and the key file
void add_tutor(const std::string& fname, const std::string& lname, const std::string& subject,
    const std::string& price, const std::string& city, const std::string& country,
    const std::string& email, const std::string& bio);

// Converts a string from a file to a vector using "|" as a delimiter
std::vector<std::string> line_to_vector(const std::string& line);

std::list<std::vector<std::string>> listSort(std::list<std::vector<std::string>> input_list, std::vector<std::string> input_vec);

std::queue<std::vector<std::string>> sortQ(std::queue<std::vector<std::string>> input);

std::string joinTutorLine(const std::vector<std::string>& tutorDetails);

// Searches the key file for a key and adds it to a queue
std::queue<std::vector<std::string>> SearchKeyFile(std::queue<std::vector<std::string>> Results, std::string key1);

// Prints a vector to the terminal
void printV(std::vector<std::string> input);

// Prints a queue of vectors to the terminal
void printQ(std::queue<std::vector<std::string>> input);

// Gets a tutor's line from the main data file based on the location
std::vector<std::string> GetTutorLine(double location);

// Gets the rating or the number of ratings from a line in the main data file
double getRating(double location, bool R_or_num);

// Inserts a rating and number of ratings into the main data file and updates the key file
void insert_New_Rating(double tutorKey, double newRating);
//Function to split the line
std::vector<std::string> splitLine(const std::string& line);

//Function to grab the rating in the right position
double extractRating(const std::string& ratingSection);

// Rates a tutor by calculating the new rating and number of ratings, and then updates the files
void Rate_Tutor(double location, double new_rate);

#endif
