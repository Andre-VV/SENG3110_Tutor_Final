#include "SearchTutor.h"
#include "Tutor_Main_F.h"
#include <wx/wx.h>
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

    // Bind events
    filterDropdown->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
        wxString selectedSubject = filterDropdown->GetStringSelection();  // Get selected subject
        string subject = selectedSubject.ToStdString();  // Convert to string for search

        // Clear the list before adding new entries
        tutorList->Clear();

        if (subject == "All") {
            // Fetch all tutors if "All" is selected
            fstream tutors("ListOfTutors_F.txt");
            string line;
            while (getline(tutors, line)) {
                vector<string> tutorDetails = line_to_vector(line, vector<string>());
                string tutorName = tutorDetails[1] + " " + tutorDetails[2];  // Full name: first + last
                tutorList->Append(tutorName);  // Add tutor name to the list
            }
            tutors.close();
        }
        else {
            // Otherwise, fetch tutors based on the subject filter
            queue<vector<string>> filteredTutors = SearchKeyFile(queue<vector<string>>(), subject);

            // Use the position details from the key file and fetch corresponding tutor details from the ListOfTutors_F.txt file
            while (!filteredTutors.empty()) {
                vector<string> tutor = filteredTutors.front();
                filteredTutors.pop();

                double position = stod(tutor[0]);  // The position from the key file

                // Fetch the full tutor details from ListOfTutors_F.txt using the position
                vector<string> tutorDetails = GetTutorLine(position);
                string tutorName = tutorDetails[1] + " " + tutorDetails[2];  // Full name: first + last
                tutorList->Append(tutorName);  // Add tutor name to the list
            }
        }
            });

    tutorList->Bind(wxEVT_LISTBOX, [=](wxCommandEvent& event) {
        // Get the index of the selected tutor in the list box
        int selectedIndex = tutorList->GetSelection();

        // If a tutor is selected with a valid index
        if (selectedIndex != wxNOT_FOUND) {
            // Fetch the subject from the dropdown to filter the tutors based on the selected subject
            wxString selectedSubject = filterDropdown->GetStringSelection();
            string subject = selectedSubject.ToStdString();

            // Create filtered tutor vector string
            queue<vector<string>> filteredTutors;
            //Check the subject if its ALL or a specific subject
            if (subject == "All") {
                // Fetch all tutors without applying any filter from the key file
                filteredTutors = SearchKeyFile(queue<vector<string>>(), "");
            }
            else {
                // Fetch the filtered tutors from the key file based on the selected subject
                filteredTutors = SearchKeyFile(queue<vector<string>>(), subject);
            }

            // Traverse through the filtered tutors and get the tutor details based on the selected index
            int currentIndex = 0;
            while (!filteredTutors.empty()) {
                vector<string> tutor = filteredTutors.front();
                filteredTutors.pop();

                // If the current index matches the selected index in the list box, fetch the full tutor details
                if (currentIndex == selectedIndex) {
                    double position = stod(tutor[0]);  // Grab the position stored as the first element in the key file entry

                    // Fetch the full tutor details from the ListOfTutors_F.txt file using the position
                    vector<string> tutorDetails = GetTutorLine(position);

                    // Construct a string with tutor details to display
                    wxString tutorInfo = "Name: " + tutorDetails[1] + " " + tutorDetails[2] + "\n" +
                        "Subject: " + tutorDetails[3] + "\n" +
                        "Price: " + tutorDetails[4] + "\n" +
                        "City: " + tutorDetails[6] + "\n" +
                        "Country: " + tutorDetails[7] + "\n" +
                        "Biography: " + tutorDetails[9] + "\n" +
                        "Rating: " + to_string(getRating(position, true)) + "\n";

                    // Display the tutor information in a message box, may want to see if this can be modified to look better
                    wxMessageBox(tutorInfo, "Tutor Information", wxOK | wxICON_INFORMATION);
                    break;
                }

                currentIndex++;
            }
        }
        });

    


    rateButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {
        // Get the selected tutor from the list
        int selectedIndex = tutorList->GetSelection();

        if (selectedIndex != wxNOT_FOUND) {
            // Get the subject from the dropdown to filter the tutors
            wxString selectedSubject = filterDropdown->GetStringSelection();
            string subject = selectedSubject.ToStdString();

            // Fetch the filtered tutors based on the selected subject or all tutors if "All" is selected
            queue<vector<string>> filteredTutors;
            if (subject == "All") {
                filteredTutors = SearchKeyFile(queue<vector<string>>(), "");
            }
            else {
                filteredTutors = SearchKeyFile(queue<vector<string>>(), subject);
            }

            // Find the tutor details corresponding to the selected index
            int currentIndex = 0;
            while (!filteredTutors.empty()) {
                vector<string> tutor = filteredTutors.front();
                filteredTutors.pop();

                if (currentIndex == selectedIndex) {
                    // Get the position of the tutor from the key file entry
                    double position = stod(tutor[0]);

                    // Fetch the full tutor details
                    vector<string> tutorDetails = GetTutorLine(position);

                    // Display tutor information in a message box
                    wxString tutorInfo = "Name: " + tutorDetails[1] + " " + tutorDetails[2] + "\n" +
                        "Subject: " + tutorDetails[3] + "\n" +
                        "Price: " + tutorDetails[4] + "\n" +
                        "City: " + tutorDetails[6] + "\n" +
                        "Country: " + tutorDetails[7] + "\n" +
                        "Biography: " + tutorDetails[9] + "\n" +
                        "Current Rating: " + to_string(getRating(position, true)) + "\n" +
                        "Number of Ratings: " + to_string(getRating(position, false)) + "\n";
                    // Display the tutor details
                    wxMessageBox(tutorInfo, "Tutor Information", wxOK | wxICON_INFORMATION);

                    // Create the dialog for entering the rating
                    wxDialog* rateDialog = new wxDialog(this, wxID_ANY, "Rate Tutor", wxDefaultPosition, wxSize(300, 150));
                    wxBoxSizer* rateSizer = new wxBoxSizer(wxVERTICAL);

                    // Create a text input control for rating
                    wxTextCtrl* ratingTextCtrl = new wxTextCtrl(rateDialog, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30), wxTE_MULTILINE);
                    rateSizer->Add(ratingTextCtrl, 0, wxALL | wxALIGN_CENTER, 10);

                    // Create the Submit button
                    wxButton* submitButton = new wxButton(rateDialog, wxID_ANY, "Submit Rating", wxDefaultPosition, wxDefaultSize);
                    rateSizer->Add(submitButton, 0, wxALL | wxALIGN_CENTER, 10);

                    // Bind the submit button to handle rating submission
                    submitButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& submitEvent) {
                        // Get the rating entered by the user
                        string ratingInput = ratingTextCtrl->GetValue().ToStdString();
                        float newRating;

                        try {
                            newRating = stof(ratingInput); // Convert input to float
                        }
                        catch (const std::invalid_argument& e) {
                            wxMessageBox("Please enter a valid rating.", "Invalid Rating", wxOK | wxICON_ERROR);
                            return; // Stop if the input is invalid and let the user try again
                        }

                        // See if the rating is between 1 and 5
                        if (newRating < 1.0f || newRating > 5.0f) {
                            //Otherwise return an error
                            wxMessageBox("Please enter a rating between 1 and 5.", "Invalid Rating", wxOK | wxICON_ERROR);
                            return;
                        }

                        // Call Rate_Tutor function to update the rating
                        Rate_Tutor(position, newRating);

                        // Notify the user that the rating has been updated
                        wxMessageBox("Rating updated successfully!", "Success", wxOK | wxICON_INFORMATION);
                        rateDialog->EndModal(wxID_OK); // Close the dialog box
                        });

                    rateDialog->SetSizer(rateSizer);
                    rateDialog->Layout();
                    rateDialog->ShowModal();
                    break; // Exit after finding the tutor
                }
                currentIndex++;
            }
        }
        else {
            // If no tutor is selected
            wxMessageBox("Please select a tutor to rate.", "Error", wxOK | wxICON_ERROR);
        }
        });
}
