#include "AddTutor.h"
#include "Tutor_Main_F.h"
#include <wx/sizer.h>
#include <wx/dialog.h>

using namespace std;



AddTutor::AddTutor(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Add Tutor", wxDefaultPosition, wxSize(400, 400)) {
    wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);

    // Input fields for tutor details
    wxTextCtrl* fnameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    wxTextCtrl* lnameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    wxTextCtrl* subjectField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    wxTextCtrl* priceField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    wxTextCtrl* cityField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    wxTextCtrl* countryField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    wxTextCtrl* emailField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    wxTextCtrl* backgroundField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));

    // Add fields to the layout
    layout->Add(new wxStaticText(this, wxID_ANY, "First Name:"), 0, wxALL, 5);
    layout->Add(fnameField, 0, wxALL | wxEXPAND, 5);
    layout->Add(new wxStaticText(this, wxID_ANY, "Last Name:"), 0, wxALL, 5);
    layout->Add(lnameField, 0, wxALL | wxEXPAND, 5);
    layout->Add(new wxStaticText(this, wxID_ANY, "Subject:"), 0, wxALL, 5);
    layout->Add(subjectField, 0, wxALL | wxEXPAND, 5);
    layout->Add(new wxStaticText(this, wxID_ANY, "Price:"), 0, wxALL, 5);
    layout->Add(priceField, 0, wxALL | wxEXPAND, 5);
    layout->Add(new wxStaticText(this, wxID_ANY, "City:"), 0, wxALL, 5);
    layout->Add(cityField, 0, wxALL | wxEXPAND, 5);
    layout->Add(new wxStaticText(this, wxID_ANY, "Country:"), 0, wxALL, 5);
    layout->Add(countryField, 0, wxALL | wxEXPAND, 5);
    layout->Add(new wxStaticText(this, wxID_ANY, "Email:"), 0, wxALL, 5);
    layout->Add(emailField, 0, wxALL | wxEXPAND, 5);
    layout->Add(new wxStaticText(this, wxID_ANY, "Biography:"), 0, wxALL, 5);
    layout->Add(backgroundField, 0, wxALL | wxEXPAND, 5);

    // Submit button
    wxButton* submitButton = new wxButton(this, wxID_ANY, "Submit");
    layout->Add(submitButton, 0, wxALL | wxALIGN_CENTER, 10);

    
    SetSizerAndFit(layout);
    

    // Bind submit button
    submitButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {
      
        // Get the input values from the fields
        string fname = fnameField->GetValue().ToStdString();
        string lname = lnameField->GetValue().ToStdString();
        string subject = subjectField->GetValue().ToStdString();
        string price = priceField->GetValue().ToStdString();
        string city = cityField->GetValue().ToStdString();
        string country = countryField->GetValue().ToStdString();
        string email = emailField->GetValue().ToStdString();
        string bio = backgroundField->GetValue().ToStdString();

        // Call add_tutor from the utility file to save the information
        add_tutor(fname, lname, subject, price, city, country, email, bio);

        wxMessageBox("Tutor added successfully!", "Success");

        Destroy();  // Close the dialog after submission
        });
}
