#ifndef TUTOR_Main_F_H
#define TUTOR_Main_F_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

// Function to add a tutor's information to the file
void add_tutor(const string& fname,
    const string& lname,
    const string& subject,
    const string& price,
    const string& city,
    const string& country,
    const string& email,
    const string& bio);

// Function to convert a string from a file to a vector
vector<string> line_to_vector(string input, vector<string> vec_interm);

// Function to search the Key List file for a specific key
queue<vector<string>> SearchKeyFile(queue<vector<string>> Results, string key1);

// Function to print a vector of strings
void printV(vector<string> input);

// Function to print a queue of vectors
void printQ(queue<vector<string>> input);

// Function to get a tutor's line based on the position in the main data file
vector<string> GetTutorLine(double location);

// Function to get the rating or number of ratings from the tutor data
double getRating(double location, bool R_or_num);

// Function to insert a new rating into the tutor data file
void insert_New_Rating(double location, float Rate, double num_rate);

// Function to rate a tutor and update their rating in the data file
void Rate_Tutor(double location, double new_rate);

#endif 
