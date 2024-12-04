#include "Tutor_Header_Final.h"

using namespace std;


//file names to use in functions
string filename1 = "ListOfTutors_F.txt";
string filename2 = "KeyList_F.txt";

void test_endline() {
    //makes sure there is an empty line at the end of the text files
    fstream tutors(filename1);
    char testing;

    if (tutors.is_open()) {
        tutors.seekp(-1, fstream::end);
        testing = tutors.get();
        if (testing == '|') {
            tutors << endl;
        }
    }
    else {
        cout << "ListOfTutors_F.txt could not be opened. test_endline" << endl;
        return;
    }
    tutors.close();
    fstream keys(filename2);
    if (keys.is_open()) {
        keys.seekp(-1, fstream::end);
        testing = keys.get();
        if (testing == '|') {
            keys << endl;
        }
    }
    else {
        cout << "KeyList_F.txt could not be opened. test_endline" << endl;
        return;
    }
    keys.close();
}

void add_tutor(const string& fname,
    const string& lname,
    const string& subject,
    const string& price,
    const string& city,
    const string& country,
    const string& email,
    const string& bio) {


    //Adds a entry to main data file and an entry to the key file
    double position;
    fstream tutors(filename1);
    
   
    if (tutors.is_open()) {
        //if file is open seek to the end and print info
        tutors.seekp(0, fstream::end);
        position = tutors.tellp();
        tutors << position << "|" << fname << "|" << lname << "|" << subject << "|" << price << "|" << "*0.0 ^0.0000^" << "|" << city << "|" << country << "|" << email << "|" << bio << "|" << endl;
    }
    else {
        cout << "ListOfTutors_F.txt could not be opened" << endl;
        return;
    }
    tutors.close();
    fstream Keys(filename2);
    
    if (Keys.is_open()) {
        //if file is open seek to end an print location of main data and key info
        Keys.seekp(0, fstream::end);
        Keys << position << "|" << subject << "|" << price << "|" << "0.0" << "|" << endl;
    }
    else {
        cout << "KeyList_F.txt could not be opened" << endl;
        return;
    }
    Keys.close();
}

vector<string> line_to_vector(string input, vector<string> vec_interm) {
    //converts a string from a file to a vector using "|" as a delimiter
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

list<vector<string>> listSort(list<vector<string>> input_list, vector<string> input_vec) {
    //adds a value to a list in the correct location so the list is sorted
    vector<string> vector1;
    list<vector<string>>::iterator point;
    string rate1;
    string rate2;
    double rate1_d;
    double rate2_d;

    rate2 = input_vec[3];
    rate2_d = stod(rate2);

    for (point = input_list.begin(); point != input_list.end(); point++) {
        //iterates through the list and inserts value 1 ahead when it is greater then the current value
        vector1 = *point;

        rate1 = vector1[3];
        rate1_d = stod(rate1);

        if (rate2_d > rate1_d) {
            input_list.insert(point, input_vec);
            return input_list;
        }
        vector1.clear();
    }
    input_list.push_back(input_vec);
    return input_list;

}

queue<vector<string>> sortQ(queue<vector<string>> input) {
    //sorts a list by rating in decending order
    //takes an unsorted queue as an input and oupputs a sorted queue
    //version of insertion sort: O(n^2)

    list<vector<string>> sortingList;
    vector<string> vector1;
    if (input.empty()) {
        return input;
    }

    vector1 = input.front();
    input.pop();
    sortingList.push_front(vector1);
    vector1.clear();

    while (!input.empty()) {
        //empties queue and pases each value to the sorting function
        vector1 = input.front();
        input.pop();
        sortingList = listSort(sortingList, vector1);
        vector1.clear();
    }
    long sortSize = sortingList.size();
    for (long i = 0; i < sortSize; i++) {
        input.push(sortingList.front());
        sortingList.pop_front();
    }
    return input;
}

queue<vector<string>> SearchKeyFile(
    queue<vector<string>> Results,
    string key1)
{
    //searches the key file for lines with a key in them and adds them to a queue
    fstream Keys(filename2);
    string test;
    vector<string> temp_vector;


    if (Keys.is_open()) {
        while (!Keys.eof())
        {
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
    Results = sortQ(Results);
    return Results;
}

void printV(vector<string> input) {
    //prints vector to the teminal
    for (int i = 0; i < input.size(); i++)
    {
        cout << input[i] << " ";
    }
    cout << endl;
}
void printQ(queue<vector<string>> input) {
    //prints a queue to the terminal using printV 
    while (!input.empty()) {
        printV(input.front());
        input.pop();
    }
}


vector<string> GetTutorLine(double location) {
    //gets a line from the main data file based on location
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
    //gets the rating or the number of rating from a line in the main data file -
    //- based on location
    //True returns rating. False returns number of ratings
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
    //inserts a rating and number of ratings into a line in the main data file
    //also changes rating in the keyfile

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
    fstream keys(filename2);
    vector<string> keyLine;
    string locationString;
    string tempLine;
    double locationKeys;
    string temp_str;
    double errorLocation;
    int errorKeys1;
    int errorKeys2;
    long errorCount = 1;



    if (keys.is_open()) {
        locationString = to_string((long)location);
        keys.seekp(0);
        getline(keys, tempLine);
        errorKeys2 = keys.tellp();
        errorKeys1 = tempLine.size();

        keys.seekp(0);
        error = errorKeys2 - errorKeys1;



        while (!keys.eof()) {
            locationKeys = keys.tellg();
            getline(keys, tempLine);
            keyLine = line_to_vector(tempLine, keyLine);
            if (keyLine[0] == locationString) {
                keyLine[3] = rate_str;
                keys.seekp(0);
                if (locationKeys < 0) {
                    locationKeys = 0;
                }
                keys.seekp(locationKeys);
                for (int i = 0; i < 4; i++) {
                    temp_str = keyLine[i];
                    keys.write(temp_str.c_str(), temp_str.size());
                    locationKeys = keys.tellp();
                    keys.seekp(locationKeys + 1);

                }
                keys.close();
                return;

            }
            errorCount++;
            keyLine.clear();

        }

    }
    else {
        cout << "KeyLift_F.txt could not be opened. insert_New_Rating" << endl;
        return;
    }
}



void Rate_Tutor(double location, double new_rate) {
    //main function to rate a tutor.
    double cur_rate;
    double num_rate;
    double new_num_rate;
    float calc;

    cur_rate = getRating(location, true);
    num_rate = getRating(location, false);

    //calculates new rating including past values
    calc = (cur_rate * num_rate) + new_rate;
    new_num_rate = num_rate + 1;
    calc = calc / (new_num_rate);
    calc = roundf(calc * 10) / 10;
    insert_New_Rating(location, calc, new_num_rate);
}