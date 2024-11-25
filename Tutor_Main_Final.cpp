#include "Tutor_Main_F.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <math.h>

using namespace std;

// file names to use in functions
string filename1 = "ListOfTutors_F.txt";
string filename2 = "KeyList_F.txt";

void add_tutor(const string& fname,
    const string& lname,
    const string& subject,
    const string& price,
    const string& city,
    const string& country,
    const string& email,
    const string& bio) {

    // Adds an entry to the main data file and an entry to the key file
    double position;
    fstream tutors(filename1);
    tutors.seekp(0, fstream::end);

    if (tutors.is_open()) {
        position = tutors.tellp();
        tutors << position << "|" << fname << "|" << lname << "|" << subject << "|"
            << price << "|" << "*0.0 ^0.0000^" << "|" << city << "|"
            << country << "|" << email << "|" << bio << "|" << endl;
    }
    else {
        cout << "ListOfTutors_F.txt could not be opened" << endl;
        return;
    }
    tutors.close();

    fstream Keys(filename2);
    Keys.seekp(0, fstream::end);

    if (Keys.is_open()) {
        Keys << position << "|" << subject << "|" << price << "|" << "0.0" << "|" << endl;
    }
    else {
        cout << "KeyList_F.txt could not be opened" << endl;
        return;
    }
    Keys.close();
}

vector<string> line_to_vector(string input, vector<string> vec_interm) {
    // Converts a string from a file to a vector using "|" as a delimiter
    int pos1 = 0;
    char cur;
    string temp_str;

    for (int i = 0; i < input.size(); i++) {
        cur = input[i];
        if (cur == '|') {
            temp_str = input.substr(pos1, i - pos1);
            vec_interm.push_back(temp_str);
            pos1 = i + 1;
        }
    }
    return vec_interm;
}

queue<vector<string>> SearchKeyFile(queue<vector<string>> Results, string key1) {
    // Searches the key file for lines with a key in them and adds them to a queue
    fstream Keys(filename2);
    string test;
    vector<string> temp_vector;

    if (Keys.is_open()) {
        while (!Keys.eof()) {
            getline(Keys, test);
            if (test.find(key1) != string::npos) {
                temp_vector = line_to_vector(test, temp_vector);
                Results.push(temp_vector);
                temp_vector.clear();
            }
        }
    }
    else {
        cout << "KeyList_F.txt could not be opened Search function" << endl;
        return Results;
    }
    Keys.close();
    return Results;
}

void printV(vector<string> input) {
    // Prints vector to the terminal
    for (int i = 0; i < input.size(); i++) {
        cout << input[i] << " ";
    }
    cout << endl;
}

void printQ(queue<vector<string>> input) {
    // Using printV prints a queue to the terminal
    while (!input.empty()) {
        printV(input.front());
        input.pop();
    }
}

vector<string> GetTutorLine(double location) {
    // Gets a line from the main data file based on location
    fstream tutors(filename1);
    string line;
    vector<string> interm;

    if (tutors.is_open()) {
        tutors.seekg(location);
        getline(tutors, line);
        interm = line_to_vector(line, interm);
    }
    else {
        cout << "ListOfTutors_F.txt could not be opened. GettutorLine" << endl;
        return interm;
    }
    tutors.close();
    return interm;
}

double getRating(double location, bool R_or_num) {
    // Gets the rating or the number of ratings from a line in the main data file
    fstream tutors(filename1);
    vector<string> interm;
    string rate_Section;
    string cur_R_str;
    string num_rates_str;
    double current_R;
    double num_rates;

    if (tutors.is_open()) {
        interm = GetTutorLine(location);
        rate_Section = interm[5];
        if (R_or_num == true) {
            cur_R_str = rate_Section.substr(1, 3);
            current_R = stod(cur_R_str);
            tutors.close();
            return current_R;
        }
        else {
            for (int i = 6; i < rate_Section.size(); i++) {
                if (rate_Section[i] == '.') {
                    num_rates_str = rate_Section.substr(6, i - 4);
                    break;
                }
            }
            num_rates = stod(num_rates_str);
            tutors.close();
            return num_rates;
        }
    }
    else {
        cout << "ListOfTutors_F.txt could not be opened. getRating" << endl;
        return 0;
    }
    tutors.close();
    return 0;
}

void insert_New_Rating(double location, float Rate, double num_rate) {
    // Inserts a rating and number of ratings into a line in the main data file
    fstream tutors(filename1);
    vector<string> interm;
    string rate_Section;
    char search;
    string rate_str;
    string num_str;
    char const* rate_char;
    char const* num_char;
    double position = 0;
    double error;
    double calc;

    if (tutors.is_open()) {
        tutors.seekg(0);
        tutors.seekp(0);
        tutors.seekg(location);
        tutors.seekp(location);
        search = tutors.get();
        calc = tutors.tellg();
        error = (calc - 1) - location;

        while (search != '*') {
            search = tutors.get();
        }

        calc = tutors.tellg();
        tutors.seekp(calc - error);

        rate_str = to_string(Rate);
        rate_str.resize(3);
        rate_char = rate_str.c_str();

        num_str = to_string(num_rate);
        num_str.resize(6);
        num_char = num_str.c_str();

        tutors.write(rate_char, 3);
        position = tutors.tellp();
        tutors.seekp(position + 2);
        tutors.write(num_char, 6);
    }
    else {
        cout << "ListOfTutors_F.txt could not be opened. insert_New_Rating" << endl;
        return;
    }
    tutors.close();
}

void Rate_Tutor(double location, double new_rate) {
    // Main function to rate a tutor
    double cur_rate;
    double num_rate;
    double new_num_rate;
    float calc;

    cur_rate = getRating(location, true);
    num_rate = getRating(location, false);

    calc = (cur_rate * num_rate) + new_rate;
    new_num_rate = num_rate + 1;
    calc = calc / (new_num_rate);
    calc = roundf(calc * 10) / 10;

    insert_New_Rating(location, calc, new_num_rate);
}
