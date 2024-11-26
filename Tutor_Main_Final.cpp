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
            << price << "|" << "*0.0^0.0000^" << "|" << city << "|" << country << "|"
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
//Gets the rating from line
double extractRating(const std::string& ratingSection) {
    size_t ratingStart = ratingSection.find('*');
    size_t ratingsStart = ratingSection.find('^');

    if (ratingStart != std::string::npos && ratingsStart != std::string::npos) {
        // Extract the rating value after the '*' and before the first '^'
        return std::stod(ratingSection.substr(ratingStart + 1, ratingsStart - ratingStart - 1));
    }
    else {
        // Return 0 if the rating format is invalid
        return 0.0;
    }
}

// Function to sort a list of vectors based on the rating value (stored in the 5th element, index 4).
std::list<std::vector<std::string>> listSort(std::list<std::vector<std::string>> input_list, std::vector<std::string> input_vec) {
    input_list.push_back(input_vec);  // Add the new vector to the list
    input_list.sort([](const std::vector<std::string>& a, const std::vector<std::string>& b) {
        // Extract the ratings from the 5th element (index 4)
        double ratingA = extractRating(a[4]);
        double ratingB = extractRating(b[4]);

        // Sort by rating in descending order
        return ratingA > ratingB;
        });
    return input_list;
}

std::queue<std::vector<std::string>> sortQ(std::queue<std::vector<std::string>> input) {
    std::list<std::vector<std::string>> sortingList;
    std::vector<std::string> vector1;

    // Process each tutor entry in the queue
    while (!input.empty()) {
        vector1 = input.front();
        input.pop();

        // Use the tutor key (first element) to get the full tutor data
        double key = std::stod(vector1[0]);  // Assuming the tutor key is in the first position

        // Fetch the tutor data based on the key
        vector1 = GetTutorLine(key); // Assuming GetTutorLine uses the tutor key to get the full tutor data

        // Ensure that vector1 has enough elements (at least 5 for the rating)
        if (vector1.size() < 5) {
            std::cout << "Missing Data! Adding missing elements." << std::endl;
            while (vector1.size() < 5) {
                vector1.push_back("0");  // Fill missing data with "0"
            }
        }

        // Now push the tutor data into sortingList
        sortingList.push_back(vector1);
    }

    // Sort the list by rating, which is in the 5th position (index 4)
    sortingList.sort([](const std::vector<std::string>& a, const std::vector<std::string>& b) {
        size_t ratingStart = a[5].find('*');
        size_t ratingsStart = a[5].find('^');
        double ratingA = std::stod(a[5].substr(ratingStart + 1, ratingsStart - ratingStart - 1));

        ratingStart = b[5].find('*');
        ratingsStart = b[5].find('^');
        double ratingB = std::stod(b[5].substr(ratingStart + 1, ratingsStart - ratingStart - 1));

        return ratingA > ratingB;  // Sort by rating in descending order
        });

    // Now convert the sorted list back to a queue and return it
    std::queue<std::vector<std::string>> sortedQueue;
    for (const auto& vec : sortingList) {
        sortedQueue.push(vec);  // Push each sorted tutor's data back into the queue
    }

    return sortedQueue;
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
        std::cout << "Running the sorting queue:" << std::endl;
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

std::vector<std::string> GetTutorLine(double tutorKey) {
    std::fstream tutors(filename1, std::ios::in);
    std::string line;
    std::vector<std::string> tutorLine;

    if (!tutors.is_open()) {
        std::cout << "ListOfTutors_F.txt could not be opened in GetTutorLine" << std::endl;
        return tutorLine;  // Return empty vector on failure
    }

    // Read through each line to find the tutor with the given tutorKey
    while (std::getline(tutors, line)) {
        // Split the line into individual parts based on a delimiter (assuming '|')
        std::vector<std::string> splitData = splitLine(line);

        if (splitData.size() > 0) {
            // Check if the first element (the key) matches the given tutorKey
            double currentTutorKey = std::stod(splitData[0]);  // Assuming the tutorKey is the first element
            if (currentTutorKey == tutorKey) {
                tutorLine = splitData;  // Found the tutor, save the line
                break;
            }
        }
    }

    tutors.close();

    if (tutorLine.empty()) {
        std::cerr << "Tutor with key " << tutorKey << " not found." << std::endl;
    }

    return tutorLine;  // Return the found tutor's data or an empty vector if not found
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
        if (i < tutorDetails.size() -1) {
            joinedLine << "|";
        }
    }

    return joinedLine.str();
}

void insert_New_Rating(double tutorKey, double newRating) {
    std::fstream tutors("ListOfTutors_F.txt", std::ios::in | std::ios::out);
    if (!tutors.is_open()) {
        std::cerr << "Error opening ListOfTutors_F.txt\n";
        return;
    }

    // Read all lines from the file into a vector
    std::string line;
    std::vector<std::string> allLines;
    while (getline(tutors, line)) {
        allLines.push_back(line);
    }
    tutors.close();

    // Loop through each tutor line to find the tutor by its unique key (tutorKey)
    bool tutorFound = false;
    for (size_t i = 0; i < allLines.size(); ++i) {
        std::vector<std::string> tutorLine = splitLine(allLines[i]);

        // Assuming that the first element of the tutorLine is the unique tutorKey as a string
        double currentTutorKey = std::stod(tutorLine[0]);  // Get the unique key (first element in the line)

        if (currentTutorKey == tutorKey) {
            tutorFound = true;

            // Ensure the tutor data is valid (i.e., has at least 5 fields including rating)
            if (tutorLine.size() <= 5) {
                std::cerr << "Invalid tutor data at key: " << tutorKey << std::endl;
                return;
            }

            // Get and update the rating
            std::string rateSection = tutorLine[5];
            size_t ratingStart = rateSection.find('*');
            size_t ratingsStart = rateSection.find('^');
            if (ratingStart == std::string::npos || ratingsStart == std::string::npos) {
                std::cerr << "Invalid rating section format: " << rateSection << std::endl;
                return;
            }

            // Parse current rating and number of ratings
            double currentRating = std::stod(rateSection.substr(ratingStart + 1, ratingsStart - ratingStart - 1));
            double numRatings = std::stod(rateSection.substr(ratingsStart + 1));

            // Calculate the updated rating and number of ratings
            double updatedRating = (currentRating * numRatings + newRating) / (numRatings + 1);
            double updatedNumRatings = numRatings + 1;

            // Create the updated rating section
            std::string updatedRateSection = "*" + std::to_string(updatedRating) + "^" + std::to_string(updatedNumRatings) + "^";

            // Update the rating section of the tutor line
            tutorLine[5] = updatedRateSection;

            // Update the corresponding line in the file
            allLines[i] = joinTutorLine(tutorLine);  // Assuming joinTutorLine is defined elsewhere to combine the tutorLine back into a single string
            break;
        }
    }

    if (!tutorFound) {
        std::cerr << "Error: Tutor with key " << tutorKey << " not found." << std::endl;
        return;
    }

    // Write the updated lines back to the file
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
    new_avg_rating = std::round((new_avg_rating * 10.0) / 10.0);

    // Debugging output
    std::cout << "Updated Rating: " << new_avg_rating << std::endl;
    std::cout << "Updated Number of Ratings: " << new_num_rate << std::endl;

    // Update the rating in the data file
    insert_New_Rating(location, new_avg_rating);
}

std::vector<std::string> splitLine(const std::string& line) {
    std::vector<std::string> result;
    std::istringstream ss(line);
    std::string token;
    while (getline(ss, token, '|')) {  // Assuming '|' is the delimiter
        result.push_back(token);
    }
    return result;
}
