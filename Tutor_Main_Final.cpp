#include "Tutor_Main_F.h"

// File names to use in functions
std::string filename1 = "ListOfTutors_F.txt";
std::string filename2 = "KeyList_F.txt";

void test_endline() {
    // Makes sure there is an empty line at the end of the text files
    std::fstream tutors(filename1);
    char testing;

    if (tutors.is_open()) {
        tutors.seekp(-1, std::fstream::end);
        testing = tutors.get();
        if (testing == '|') {
            tutors << std::endl;
        }
    }
    else {
        std::cout << "ListOfTutors_F.txt could not be opened. test_endline" << std::endl;
        return;
    }
    tutors.close();
    std::fstream keys(filename2);
    if (keys.is_open()) {
        keys.seekp(-1, std::fstream::end);
        testing = keys.get();
        if (testing == '|') {
            keys << std::endl;
        }
    }
    else {
        std::cout << "KeyList_F.txt could not be opened. test_endline" << std::endl;
        return;
    }
    keys.close();
}

void add_tutor(const std::string& fname, const std::string& lname, const std::string& subject,
    const std::string& price, const std::string& city, const std::string& country,
    const std::string& email, const std::string& bio) {
    test_endline();
    double position;
    std::fstream tutors(filename1);
    tutors.seekp(0, std::fstream::end);
    std::cout << "Function running" << std::endl;
    if (tutors.is_open()) {
        position = tutors.tellp();
        tutors << position << "|" << fname << "|" << lname << "|" << subject << "|"
            << price << "|" << "*0.0 ^0.0000^" << "|" << city << "|" << country << "|"
            << email << "|" << bio << "|" << std::endl;
    }
    else {
        std::cout << "ListOfTutors_F.txt could not be opened" << std::endl;
        return;
    }
    tutors.close();

    std::fstream keys(filename2);
    keys.seekp(0, std::fstream::end);
    if (keys.is_open()) {
        keys << position << "|" << subject << "|" << price << "|" << "0.0" << "|" << std::endl;
    }
    else {
        std::cout << "KeyList_F.txt could not be opened" << std::endl;
        return;
    }
    keys.close();
}

std::vector<std::string> line_to_vector(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, '|')) {
        fields.push_back(field);
    }

    // Debug log
    for (size_t i = 0; i < fields.size(); ++i) {
        std::cout << "Field " << i << ": " << fields[i] << std::endl;
    }

    return fields;
}

std::list<std::vector<std::string>> listSort(std::list<std::vector<std::string>> input_list, std::vector<std::string> input_vec) {
    input_list.push_back(input_vec);
    input_list.sort([](const std::vector<std::string>& a, const std::vector<std::string>& b) {
        return std::stod(a[3]) > std::stod(b[3]); // Sort by rating in descending order
        });
    return input_list;
}

std::queue<std::vector<std::string>> sortQ(std::queue<std::vector<std::string>> input) {
    std::list<std::vector<std::string>> sortingList;
    std::vector<std::string> vector1;

    while (!input.empty()) {
        vector1 = input.front();
        input.pop();

        if (vector1.size() <= 3) {
            while (vector1.size() < 4) {
                vector1.push_back("0");
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

std::queue<std::vector<std::string>> SearchKeyFile(std::queue<std::vector<std::string>> Results, std::string key1) {
    std::fstream keys(filename2);
    std::string line;
    std::vector<std::string> temp_vector;

    if (!keys.is_open()) {
        std::cout << "KeyList_F.txt could not be opened in SearchKeyFile" << std::endl;
        return Results;
    }

    while (std::getline(keys, line)) {
        if (key1 == "all" || line.find(key1) != std::string::npos) {
            temp_vector = line_to_vector(line);
            Results.push(temp_vector);
        }
    }

    keys.close();

    if (Results.empty()) {
        std::cout << "No matching results found in SearchKeyFile." << std::endl;
    }
    else if (Results.size() > 1 && key1 != "all") {
        Results = sortQ(Results);
    }

    return Results;
}
/*
void printV(const std::vector<std::string>& input) {
    for (const auto& elem : input) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

void printQ(std::queue<std::vector<std::string>> input) {
    while (!input.empty()) {
        printV(input.front());
        input.pop();
    }
}*/

std::vector<std::string> GetTutorLine(double location) {
    std::fstream tutors(filename1);
    std::string line;
    std::vector<std::string> interm;

    if (tutors.is_open()) {
        tutors.seekg(location);
        std::getline(tutors, line);
        interm = line_to_vector(line);
    }
    else {
        std::cout << "ListOfTutors_F.txt could not be opened in GetTutorLine" << std::endl;
    }
    tutors.close();
    return interm;
}

double getRating(double location, bool R_or_num) {
    std::fstream tutors("ListOfTutors_F.txt");

    if (!tutors.is_open()) {
        std::cerr << "Error opening ListOfTutors_F.txt\n";
        return 0.0;
    }

    std::vector<std::string> tutorDetails = GetTutorLine(location);
    if (tutorDetails.size() <= 5) {
        std::cerr << "Invalid tutor data at location: " << location << std::endl;
        return 0.0;
    }

    std::string rateSection = tutorDetails[5];
    size_t ratingStart = rateSection.find('*');
    size_t ratingsStart = rateSection.find('^');

    if (ratingStart == std::string::npos || ratingsStart == std::string::npos) {
        std::cerr << "Invalid rating section format: " << rateSection << std::endl;
        return 0.0;
    }

    if (R_or_num) {
        return std::stod(rateSection.substr(ratingStart + 1, ratingsStart - ratingStart - 1));
    }
    else {
        return std::stod(rateSection.substr(ratingsStart + 1));
    }
}

std::string joinTutorLine(const std::vector<std::string>& tutorDetails) {
    std::stringstream joinedLine;

    for (size_t i = 0; i < tutorDetails.size(); ++i) {
        joinedLine << tutorDetails[i];
        if (i < tutorDetails.size() - 1) {
            joinedLine << "|";
        }
    }

    return joinedLine.str();
}

void insert_New_Rating(double location, double newRating) {
    std::fstream tutors("ListOfTutors_F.txt", std::ios::in | std::ios::out);
    if (!tutors.is_open()) {
        std::cerr << "Error opening ListOfTutors_F.txt\n";
        return;
    }

    std::vector<std::string> tutorLine = GetTutorLine(location);
    if (tutorLine.size() <= 5) {
        std::cerr << "Invalid tutor data at location: " << location << std::endl;
        return;
    }

    std::string rateSection = tutorLine[5];
    size_t ratingStart = rateSection.find('*');
    size_t ratingsStart = rateSection.find('^');
    if (ratingStart == std::string::npos || ratingsStart == std::string::npos) {
        std::cerr << "Invalid rating section format: " << rateSection << std::endl;
        return;
    }

    double currentRating = std::stod(rateSection.substr(ratingStart + 1, ratingsStart - ratingStart - 1));
    double numRatings = std::stod(rateSection.substr(ratingsStart + 1));

    double updatedRating = (currentRating * numRatings + newRating) / (numRatings + 1);
    double updatedNumRatings = numRatings + 1;

    std::string updatedRateSection = "*" + std::to_string(updatedRating) + "^" + std::to_string(updatedNumRatings) +
        std::to_string(updatedNumRatings) + "^";
    // Update the rating section
    tutorLine[5] = updatedRateSection;

    // Update the file by reading all lines into a vector
    tutors.seekp(0, std::ios::beg);  // Move the file pointer to the start
    std::string line;
    std::vector<std::string> allLines;
    while (getline(tutors, line)) {
        allLines.push_back(line);
    }
    tutors.close();

    // Replace the specific line (location) in the file with the updated tutor data
    if (location >= 0 && location < allLines.size()) {
        allLines[location] = joinTutorLine(tutorLine);  // Join the vector back into a single line
    }
    else {
        std::cerr << "Error: Invalid location." << std::endl;
        return;
    }

    // Write the updated data back to the file
    std::fstream outFile("ListOfTutors_F.txt", std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Error opening ListOfTutors_F.txt for writing\n";
        return;
    }

    // Write each updated line back into the file, maintaining the correct format with '|'
    for (const std::string& updatedLine : allLines) {
        outFile << updatedLine << std::endl;
    }
    outFile.close();
}


void Rate_Tutor(double location, double new_rate) {
    double cur_rate = getRating(location, true);
    double num_rate = getRating(location, false);

    // Debugging output to verify the values retrieved
    std::cout << "Current Rating: " << cur_rate << std::endl;
    std::cout << "Number of Ratings: " << num_rate << std::endl;
    std::cout << "New Rating: " << new_rate << std::endl;

    // Validate inputs
    if (cur_rate < 0 || num_rate < 0 || new_rate < 0) {
        std::cout << "Invalid rating values encountered." << std::endl;
        return;
    }

    // Calculate the new average rating
    double total_rating_sum = (cur_rate * num_rate) + new_rate;
    double new_num_rate = num_rate + 1;

    if (new_num_rate == 0) { // Shouldn't happen, but safety check
        std::cout << "Error: Division by zero when calculating the new rating." << std::endl;
        return;
    }

    double new_avg_rating = total_rating_sum / new_num_rate;

    // Round to one decimal place
    new_avg_rating = std::round(new_avg_rating * 10.0) / 10.0;

    // Debugging output
    std::cout << "Updated Rating: " << new_avg_rating << std::endl;
    std::cout << "Updated Number of Ratings: " << new_num_rate << std::endl;

    // Update the rating in the data file
    insert_New_Rating(location, new_avg_rating);
}
