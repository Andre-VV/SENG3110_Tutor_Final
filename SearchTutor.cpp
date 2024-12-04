#include "SearchTutor.h"
#include "Tutor_Header_Final.h"
#include <wx/wx.h>
#include <sstream>
using namespace std;
SearchTutor::SearchTutor(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Search Tutors", wxDefaultPosition, wxSize(400, 300)) {

    wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);

    // Dropdown for filtering tutors
    wxArrayString filterOptions;
    filterOptions.Add("All");
    filterOptions.Add("Math");
    filterOptions.Add("Science");
    filterOptions.Add("Chemistry");
    wxChoice* filterDropdown = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, filterOptions);
    layout->Add(filterDropdown, 0, wxALL | wxEXPAND, 10);

    // Tutor list
    wxListBox* tutorList = new wxListBox(this, wxID_ANY);
    layout->Add(tutorList, 1, wxALL | wxEXPAND, 10);

    // Rating button
    wxButton* rateButton = new wxButton(this, wxID_ANY, "Rate Tutor");
    layout->Add(rateButton, 0, wxALL | wxALIGN_CENTER, 10);

    SetSizer(layout);

    // Bind filterDropdown event
    filterDropdown->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
        wxString selectedSubject = filterDropdown->GetStringSelection();
        std::string subject = selectedSubject.ToStdString();

        tutorList->Clear(); // Clear the list before adding new entries

        if (subject == "All") {
            PopulateAllTutors(tutorList);
        }
        else {
            PopulateFilteredTutors(tutorList, subject);
        }
        });

    // Bind tutorList selection event
    tutorList->Bind(wxEVT_LISTBOX, [=](wxCommandEvent& event) {
        int selectedIndex = tutorList->GetSelection();
        if (selectedIndex == wxNOT_FOUND) {
            wxMessageBox("Please select a tutor to view details.", "Error", wxICON_ERROR);
            return;
        }

        DisplayTutorDetails(tutorList, selectedIndex, filterDropdown);
        });

    // Bind rateButton event
    rateButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {
        int selectedIndex = tutorList->GetSelection();
        if (selectedIndex == wxNOT_FOUND) {
            wxMessageBox("Please select a tutor to rate.", "Error", wxICON_ERROR);
            return;
        }

        RateTutor(tutorList, selectedIndex, filterDropdown);
        });
}
//Function to show all tutors, sorted since it doesn't look right otherwise
void SearchTutor::PopulateAllTutors(wxListBox* tutorList) {
    std::ifstream tutorsFile("ListOfTutors_F.txt");
    if (!tutorsFile.is_open()) {
        wxMessageBox("Could not open tutors file.", "Error", wxICON_ERROR);
        return;
    }

    std::vector<std::vector<std::string>> tutors;

    // Read all tutor data into a vector
    std::string line;
    while (std::getline(tutorsFile, line)) {
        std::vector<std::string> tutorDetails = line_to_vector(line,tutorDetails);
        if (tutorDetails.size() > 5) {
            tutors.push_back(tutorDetails);  // Store each tutor's details
        }
    }

    // Close the file
    tutorsFile.close();

    // Sort the tutors based on the rating in the 5th column 
    std::sort(tutors.begin(), tutors.end(), [](const std::vector<std::string>& a, const std::vector<std::string>& b) {
        // Extract the ratings from the 5th column 
        size_t ratingStartA = a[5].find('*');
        size_t ratingEndA = a[5].find('^');
        double ratingA = std::stod(a[5].substr(ratingStartA + 1, ratingEndA - ratingStartA - 1));

        size_t ratingStartB = b[5].find('*');
        size_t ratingEndB = b[5].find('^');
        double ratingB = std::stod(b[5].substr(ratingStartB + 1, ratingEndB - ratingStartB - 1));

        return ratingA > ratingB;  // Sort in descending order of ratings
        });

    // Clear the existing list box and append sorted tutor names
    tutorList->Clear();
    for (const auto& tutor : tutors) {
        std::string tutorName = tutor[1] + " " + tutor[2];  // Full name (First name + Last name)
        tutorList->Append(tutorName);
    }
}

// Function to populate tutors filtered by subject
void SearchTutor::PopulateFilteredTutors(wxListBox* tutorList, const std::string& subject) {
    std::queue<std::vector<std::string>> filteredTutors = SearchKeyFile({}, subject);

    while (!filteredTutors.empty()) {
        std::vector<std::string> tutor = filteredTutors.front();
        filteredTutors.pop();

        double position = std::stod(tutor[0]);
        std::vector<std::string> tutorDetails = GetTutorLine(position);

        if (tutorDetails.size() > 2) {
            std::string tutorName = tutorDetails[1] + " " + tutorDetails[2];
            tutorList->Append(tutorName);
        }
        tutorDetails.clear();
        tutor.clear();
    }
}

// Function to display tutor details
void SearchTutor::DisplayTutorDetails(wxListBox* tutorList, int selectedIndex, wxChoice* filterDropdown) {
    wxString selectedSubject = filterDropdown->GetStringSelection();
    std::string subject = selectedSubject.ToStdString();

    std::queue<std::vector<std::string>> filteredTutors = (subject == "All") ? SearchKeyFile({}, ".") : SearchKeyFile({}, subject);

    int currentIndex = 0;
    while (!filteredTutors.empty()) {
        std::vector<std::string> tutor = filteredTutors.front();
        filteredTutors.pop();

        if (currentIndex == selectedIndex) {
            double position = std::stod(tutor[0]);
            std::vector<std::string> tutorDetails = GetTutorLine(position);

            wxString tutorInfo = wxString::Format(
                "Name: %s\nSubject: %s\nPrice: %s\nCity: %s\nCountry: %s\nEmail: %s\nBiography: %s\nRating: %.1f",
                tutorDetails.size() > 2 ? (tutorDetails[1] + " " + tutorDetails[2]) : "Unknown",
                tutorDetails.size() > 3 ? tutorDetails[3] : "Unknown",
                tutorDetails.size() > 4 ? tutorDetails[4] : "Unknown",
                tutorDetails.size() > 6 ? tutorDetails[6] : "Unknown",
                tutorDetails.size() > 7 ? tutorDetails[7] : "Unknown",
                tutorDetails.size() > 8 ? tutorDetails[8] : "Unknown",
                tutorDetails.size() > 9 ? tutorDetails[9] : "Unknown",
                getRating(position, true)
            );

            wxMessageBox(tutorInfo, "Tutor Information", wxOK | wxICON_INFORMATION);
            break;
        }

        currentIndex++;
    }
}

// Function to rate a tutor
void SearchTutor::RateTutor(wxListBox* tutorList, int selectedIndex, wxChoice* filterDropdown) {
    wxString selectedSubject = filterDropdown->GetStringSelection();
    std::string subject = selectedSubject.ToStdString();

    std::queue<std::vector<std::string>> filteredTutors = (subject == "All") ? SearchKeyFile({}, ".") : SearchKeyFile({}, subject);
    test_endline();

    int currentIndex = 0;
    while (!filteredTutors.empty()) {
        std::vector<std::string> tutor = filteredTutors.front();
        filteredTutors.pop();

        if (currentIndex == selectedIndex) {
            double position = std::stod(tutor[0]);

            wxDialog* rateDialog = new wxDialog(this, wxID_ANY, "Rate Tutor", wxDefaultPosition, wxSize(300, 150));
            wxBoxSizer* rateSizer = new wxBoxSizer(wxVERTICAL);

            wxTextCtrl* ratingTextCtrl = new wxTextCtrl(rateDialog, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
            rateSizer->Add(ratingTextCtrl, 0, wxALL | wxALIGN_CENTER, 10);

            wxButton* submitButton = new wxButton(rateDialog, wxID_ANY, "Submit Rating");
            rateSizer->Add(submitButton, 0, wxALL | wxALIGN_CENTER, 10);

            submitButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& submitEvent) {
                std::string ratingInput = ratingTextCtrl->GetValue().ToStdString();
                double newRating;

                try {
                    newRating = std::stod(ratingInput);
                }
                catch (...) {
                    wxMessageBox("Invalid rating. Enter a number between 1 and 5.", "Error", wxICON_ERROR);
                    return;
                }

                if (newRating < 1.0 || newRating > 5.0) {
                    wxMessageBox("Rating must be between 1 and 5.", "Error", wxICON_ERROR);
                    return;
                }

                Rate_Tutor(position, newRating);
                wxMessageBox("Rating updated successfully!", "Success", wxICON_INFORMATION);
                rateDialog->EndModal(wxID_OK);
                });

            rateDialog->SetSizer(rateSizer);
            rateDialog->ShowModal();
            break;
        }
        tutor.clear();

        currentIndex++;
    }


}
