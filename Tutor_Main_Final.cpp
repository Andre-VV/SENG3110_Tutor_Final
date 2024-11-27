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
    test_endline();
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

vector<string> line_to_vector(const string& line) {
    vector<string> fields;
    stringstream ss(line);
    string field;
    while (getline(ss, field, '|')) {  // Assuming fields are comma-separated
        fields.push_back(field);
    }

    // Debug log
    for (size_t i = 0; i < fields.size(); ++i) {
        cout << "Field " << i << ": " << fields[i] << endl;
    }

    return fields;
}





list<vector<string>> listSort(list<vector<string>> input_list, vector<string> input_vec) {
    //Simplified function
    input_list.push_back(input_vec); // Insert item into the list
    input_list.sort([](const vector<string>& a, const vector<string>& b) {
        return stod(a[3]) > stod(b[3]); // Sort by rating in descending order
        });
    return input_list;
}




queue<vector<string>> sortQ(queue<vector<string>> input) {
    // Sorts a list by rating in descending order
    // Takes an unsorted queue as input and outputs a sorted queue
    list<vector<string>> sortingList;
    vector<string> vector1;

    while (!input.empty()) {
        vector1 = input.front();
        input.pop();

        // Ensure vector1 has at least 4 elements by adding default values
        if (vector1.size() <= 3) {
            while (vector1.size() < 4) {
                vector1.push_back("0"); // Fill missing elements with default value "0"
            }
        }

        sortingList = listSort(sortingList, vector1);
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
    else
    {
        cout << "Should just output results here" << endl;
        return Results;
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
        if (i < tutorDetails.size() -1) {  // Avoid appending '|' to the last element
            joinedLine << "|";
        }
        
    }

    //joinedLine << "\n"; // Add a newline at the end
    return joinedLine.str();
}


void insert_New_Rating(double location, double newRating) {
    //test_endline();
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

    // Additional validation here: ensure that ratings are within a valid range
    if (currentRating < 0 || numRatings < 0) {
        cerr << "Invalid rating or number of ratings" << endl;
        return;
    }


    double updatedRating = (currentRating * numRatings + newRating) / (numRatings + 1);
    double updatedNumRatings = numRatings + 1;

    // Format the updated rating section
    string updatedRateSection = "*" + to_string(updatedRating) + "^" + to_string(updatedNumRatings) + "^";
    // Update the rating section
    tutorLine[5] = updatedRateSection;

    // Update the file by reading all lines into a vector
    tutors.seekp(0, ios::beg);  // Move the file pointer to the start
    string line;
    vector<string> allLines;
    while (getline(tutors, line)) {
        allLines.push_back(line);
    }
    tutors.close();

    // Replace the specific line (location) in the file with the updated tutor data
    if (location >= 0 && location < allLines.size()) {
        allLines[location] = joinTutorLine(tutorLine);  // Join the vector back into a single line
    }
    else {
        cerr << "Error: Invalid location." << endl;
        return;
    }

    // Write the updated data back to the file
    fstream outFile("ListOfTutors_F.txt", ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cerr << "Error opening ListOfTutors_F.txt for writing\n";
        return;
    }

    // Write each updated line back into the file, maintaining the correct format with '|'
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
