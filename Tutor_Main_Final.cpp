#include "Tutor_Main_F.h"

// File names to use in functions
string filename1 = "ListOfTutors_F.txt";
string filename2 = "KeyList_F.txt";


void test_endline() {
    //makes sure there is an empty line at the end of the text files
    fstream tutors(filename1);
    char testing;

    if (tutors.is_open()) {
        tutors.seekp(-1, fstream::end);
        testing = tutors.get();
        //cout<<"test: "<<testing<<endl;
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

vector<string> line_to_vector(const string& input) {
    vector<string> vec_interm;
    size_t pos1 = 0;
    char cur;
    string temp_str;

    // Iterate over the string to extract tokens
    for (size_t i = 0; i < input.size(); ++i) {
        cur = input[i];
        if (cur == '|') {
            temp_str = input.substr(pos1, i - pos1);
            vec_interm.push_back(temp_str);
            pos1 = i + 1;  // Move position to the character after '|'
        }
    }

    // Handle the last token after the last '|'
    if (pos1 < input.size()) {
        temp_str = input.substr(pos1);  // Get the remaining substring
        vec_interm.push_back(temp_str);
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
    list<vector<string>> sortingList;
    vector<string> vector1;

    vector1 = input.front();
    input.pop();
    sortingList.push_front(vector1);
    vector1.clear();

    while (!input.empty()) {
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


queue<vector<string>> SearchKeyFile(queue<vector<string>> Results, string key1) {
    fstream Keys(filename2); // Open the file containing the keys
    string line;
    vector<string> temp_vector;

    if (!Keys.is_open()) {
        cout << "KeyList_F.txt could not be opened in SearchKeyFile" << endl;
        return Results; // Return empty or unmodified Results if file cannot be opened
    }

    // Process each line in the file
    while (getline(Keys, line)) {
        if (key1 == "all" || line.find(key1) != string::npos) {
            // Convert the line to a vector and add it to the queue
            temp_vector = line_to_vector(line);
            Results.push(temp_vector);
            //temp_vector.clear();
        }
    }

    Keys.close(); // Close the file

    // Handle cases based on the size of the results
    if (Results.empty()) {
        cout << "No matching results found in SearchKeyFile." << endl;
    }
    else if (Results.size() > 1 && key1 != "all") {
        // Sort the results only if there are multiple items and key1 is not "all"
        Results = sortQ(Results);
    }

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
        interm = line_to_vector(line);
    }
    else {
        cout << "ListOfTutors_F.txt could not be opened in GetTutorLine" << endl;
        return interm;
    }
    tutors.close();
    return interm;
}

double getRating(double location, bool R_or_num) {
    fstream tutors("ListOfTutors_F.txt");
    if (!tutors.is_open()) {
        cerr << "Error opening ListOfTutors_F.txt\n";
        return 0.0;
    }

    vector<string> tutorDetails = GetTutorLine(location);
    if (tutorDetails.size() <= 5) {
        cerr << "Invalid tutor data at location: " << location << endl;
        return 0.0;
    }

    string rateSection = tutorDetails[5]; // Ensure this is correctly parsed
    size_t ratingStart = rateSection.find('*');
    size_t ratingsStart = rateSection.find('^');

    if (ratingStart == string::npos || ratingsStart == string::npos) {
        cerr << "Invalid rating section format: " << rateSection << endl;
        return 0.0;
    }

    if (R_or_num) {
        // Extract rating
        return stod(rateSection.substr(ratingStart + 1, ratingsStart - ratingStart - 1));
    }
    else {
        // Extract number of ratings
        return stod(rateSection.substr(ratingsStart + 1));
    }
}


string joinTutorLine(const vector<string>& tutorDetails) {
    stringstream joinedLine;

    for (size_t i = 0; i < tutorDetails.size(); ++i) {
        joinedLine << tutorDetails[i];
        if (i < tutorDetails.size()) {  // Avoid appending '|' to the last element
            joinedLine << "|";
        }
        joinedLine << "|";
    }

    joinedLine << "\n"; // Add a newline at the end
    return joinedLine.str();
}


void insert_New_Rating(double location, double newRating) {
    fstream tutors("ListOfTutors_F.txt", ios::in | ios::out);
    if (!tutors.is_open()) {
        cerr << "Error opening ListOfTutors_F.txt\n";
        return;
    }

    // Convert location to an integer index (if necessary)
    int index = static_cast<int>(location);  // Assuming location is meant to be an index

    vector<string> tutorLine = GetTutorLine(index);
    if (tutorLine.size() <= 5) {
        cerr << "Invalid tutor data at location: " << index << endl;
        return;
    }

    string rateSection = tutorLine[5];
    size_t ratingStart = rateSection.find('*');
    size_t ratingsStart = rateSection.find('^');
    if (ratingStart == string::npos || ratingsStart == string::npos) {
        cerr << "Invalid rating section format: " << rateSection << endl;
        return;
    }

    double currentRating = stod(rateSection.substr(ratingStart + 1, ratingsStart - ratingStart - 1));
    double numRatings = stod(rateSection.substr(ratingsStart + 1));

    double updatedRating = (currentRating * numRatings + newRating) / (numRatings + 1);
    double updatedNumRatings = numRatings + 1;

    // Update the rating section
    tutorLine[5] = "*" + to_string(updatedRating) + "^" + to_string(updatedNumRatings) + "^";

    // Read all lines into a vector
    string line;
    vector<string> allLines;
    while (getline(tutors, line)) {
        allLines.push_back(line);
    }
    tutors.close();

    // Check bounds before modifying the line
    if (index >= 0 && index < allLines.size()) {
        allLines[index] = joinTutorLine(tutorLine);  // Ensure joinTutorLine works correctly
    }
    else {
        cerr << "Error: Invalid location." << endl;
        return;
    }

    // Open file in write mode and overwrite with updated data
    fstream outFile("ListOfTutors_F.txt", ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cerr << "Error opening ListOfTutors_F.txt for writing\n";
        return;
    }

    for (const string& updatedLine : allLines) {
        outFile << updatedLine << endl;
    }
    outFile.close();
}


void Rate_Tutor(double location, double new_rate) {
    double cur_rate = getRating(location, true);
    double num_rate = getRating(location, false);

    // Debugging output to verify the values retrieved
    cout << "Current Rating: " << cur_rate << endl;
    cout << "Number of Ratings: " << num_rate << endl;
    cout << "New Rating: " << new_rate << endl;

    // Validate inputs
    if (cur_rate < 0 || num_rate < 0 || new_rate < 0) {
        cout << "Invalid rating values encountered." << endl;
        return;
    }

    // Calculate the new average rating
    double total_rating_sum = (cur_rate * num_rate) + new_rate;
    double new_num_rate = num_rate + 1;

    if (new_num_rate == 0) { // Shouldn't happen, but safety check
        cout << "Error: Division by zero when calculating the new rating." << endl;
        return;
    }

    double new_avg_rating = total_rating_sum / new_num_rate;

    // Round to one decimal place
    new_avg_rating = round(new_avg_rating * 10.0) / 10.0;

    // Debugging output
    cout << "Updated Rating: " << new_avg_rating << endl;
    cout << "Updated Number of Ratings: " << new_num_rate << endl;

    // Update the rating in the data file
    insert_New_Rating(location, new_avg_rating);
}
