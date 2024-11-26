#include "Tutor_Main_F.h"

// File names to use in functions
string filename1 = "ListOfTutors_F.txt";
string filename2 = "KeyList_F.txt";

// Function Definitions

void add_tutor(const string& fname, const string& lname, const string& subject,
    const string& price, const string& city, const string& country,
    const string& email, const string& bio) {

    double position;
    fstream tutors(filename1);
    tutors.seekp(0, fstream::end);
    cout << "Function running" << endl;
    if (tutors.is_open()) {
        position = tutors.tellp();
        tutors << position << "|" << fname << "|" << lname << "|" << subject << "|"
            << price << "|" << "*0.0 ^0.0000^" << "|" << city << "|" << country << "|"
            << email << "|" << bio << "|" << endl;
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
    list<vector<string>>::iterator point;
    double rate2_d = 0.0;

    // Validate input_vec size before accessing index 5
    if (input_vec.size() > 5) {
        string rate2_section = input_vec[5];

        // Extract and parse rating from input_vec[5]
        try {
            size_t start = rate2_section.find('*') + 1;   // Find the start of the rating
            size_t end = rate2_section.find('^');        // Find the end of the rating
            rate2_d = stod(rate2_section.substr(start, end - start)); // Extract and convert
        }
        catch (const exception& e) {
            rate2_d = 0.0; // Default rating if invalid
        }
    }
    else {
        rate2_d = 0.0; // Default rating if input_vec is malformed
    }

    for (point = input_list.begin(); point != input_list.end(); ++point) {
        double rate1_d = 0.0;

        // Validate current list element size before accessing index 5
        if ((*point).size() > 5) {
            string rate1_section = (*point)[5];

            // Extract and parse rating from current list element
            try {
                size_t start = rate1_section.find('*') + 1;
                size_t end = rate1_section.find('^');
                rate1_d = stod(rate1_section.substr(start, end - start));
            }
            catch (const exception& e) {
                rate1_d = 0.0; // Default rating if invalid
            }
        }
        else {
            rate1_d = 0.0; // Default rating if list element is malformed
        }

        // Insert input_vec if its rating is greater
        if (rate2_d > rate1_d) {
            input_list.insert(point, input_vec);
            return input_list;
        }
    }

    // Append to the back if no higher rating is found
    input_list.push_back(input_vec);
    return input_list;
}




queue<vector<string>> sortQ(queue<vector<string>> input) {
    list<vector<string>> sortingList;

    // Initialize sorting list with the first element
    if (!input.empty()) {
        sortingList.push_back(input.front());
        input.pop();
    }

    // Sort remaining elements
    while (!input.empty()) {
        vector<string> vector1 = input.front();
        input.pop();
        sortingList = listSort(sortingList, vector1);
    }

    // Rebuild the queue from the sorted list
    for (const auto& vec : sortingList) {
        input.push(vec);
    }

    return input;
}



queue<vector<string>> SearchKeyFile(queue<vector<string>> Results, string key1) {
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
        cout << "KeyList_F.txt could not be opened in Search function" << endl;
        return Results;
    }
    Keys.close();
    Results = sortQ(Results);
    return Results;
}

void printV(vector<string> input) {
    for (int i = 0; i < input.size(); i++) {
        cout << input[i] << " ";
    }
    cout << endl;
}

void printQ(queue<vector<string>> input) {
    while (!input.empty()) {
        printV(input.front());
        input.pop();
    }
}

vector<string> GetTutorLine(double location) {
    fstream tutors(filename1);
    string line;
    vector<string> interm;

    if (tutors.is_open()) {
        tutors.seekg(location);
        getline(tutors, line);
        interm = line_to_vector(line, interm);
    }
    else {
        cout << "ListOfTutors_F.txt could not be opened in GetTutorLine" << endl;
        return interm;
    }
    tutors.close();
    return interm;
}

double getRating(double location, bool R_or_num) {
    // Opens the main data file to fetch tutor rating or number of ratings
    vector<string> interm;
    string rate_Section;
    string cur_R_str;
    string num_rates_str;
    double current_R = 0.0;
    double num_rates = 0.0;

    // Fetch the tutor line using the provided location
    interm = GetTutorLine(location);
    if (interm.empty()) {
        cout << "Error: Could not retrieve tutor details at the specified location." << endl;
        return 0;
    }

    // Extract the ratings section
    if (interm.size() > 5) {
        rate_Section = interm[5];

        // Split the ratings section into rating and number of ratings
        size_t separatorPos = rate_Section.find('|');
        if (separatorPos != string::npos) {
            cur_R_str = rate_Section.substr(0, separatorPos);       // Part before '|'
            num_rates_str = rate_Section.substr(separatorPos + 1); // Part after '|'
        }
        else {
            cout << "Error: Malformed ratings section: " << rate_Section << endl;
            return 0;
        }

        // Parse rating and number of ratings
        try {
            current_R = cur_R_str.empty() ? 0.0 : stod(cur_R_str);
            num_rates = num_rates_str.empty() ? 0.0 : stod(num_rates_str);
        }
        catch (const invalid_argument& e) {
            cout << "Error: Invalid rating format. " << e.what() << endl;
            return 0;
        }
        catch (const out_of_range& e) {
            cout << "Error: Rating value out of range. " << e.what() << endl;
            return 0;
        }
    }

    // Return the requested value
    return R_or_num ? current_R : num_rates;
}


void insert_New_Rating(double location, float Rate, double num_rate) {
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
        cout << "ListOfTutors_F.txt could not be opened in insert_New_Rating" << endl;
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
                locationKeys = locationKeys - (error - errorCount);
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
        cout << "KeyList_F.txt could not be opened in insert_New_Rating" << endl;
        return;
    }
}

void Rate_Tutor(double location, double new_rate) {
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
