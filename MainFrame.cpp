#include "MainFrame.h"
#include "SearchTutor.h"
#include "AddTutor.h"
#include <wx/wx.h>
#include <iostream>
#include <sstream>

class wxStreamBuf : public std::streambuf {
public:
    wxStreamBuf(wxTextCtrl* textCtrl) : m_textCtrl(textCtrl) {}

protected:
    virtual int_type overflow(int_type c) {
        if (c != EOF) {
            m_textCtrl->AppendText(wxString::Format("%c", (char)c));

            // Scroll to the bottom to ensure the latest text is visible
            m_textCtrl->ShowPosition(m_textCtrl->GetLastPosition());
        }
        return c;
    }

private:
    wxTextCtrl* m_textCtrl;
};



//Creates the main boxes and buttons to be clicked upon
MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Tutor Management", wxDefaultPosition, wxSize(400, 300)) {
    wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);

    wxButton* searchButton = new wxButton(this, wxID_ANY, "Search for Tutors");
    wxButton* addButton = new wxButton(this, wxID_ANY, "Add a Tutor");

    // Create a wxTextCtrl to display output
    wxTextCtrl* outputTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(600, 200),
        wxTE_MULTILINE | wxTE_READONLY);
    //Buttons and text to layout
    layout->Add(searchButton, 0, wxALL | wxEXPAND, 10);
    layout->Add(addButton, 0, wxALL | wxEXPAND, 10);
    layout->Add(outputTextCtrl, 1, wxALL | wxEXPAND, 10);

    searchButton->Bind(wxEVT_BUTTON, &MainFrame::OnSearchTutors, this);
    addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddTutor, this);

    SetSizer(layout);
    // Redirect std::cout and std::cerr to the wxTextCtrl
    RedirectOutputToTextCtrl(outputTextCtrl);
}
//Search tutor button
void MainFrame::OnSearchTutors(wxCommandEvent& event) {
    SearchTutor* dialog = new SearchTutor(this);
    dialog->ShowModal();
}
//Add tutor button
void MainFrame::OnAddTutor(wxCommandEvent& event) {
    AddTutor* dialog = new AddTutor(this);
    dialog->ShowModal();
}

// Redirect output to the wxTextCtrl
void MainFrame::RedirectOutputToTextCtrl(wxTextCtrl* textCtrl) {
    // Create a custom stream buffer to redirect output to the wxTextCtrl
    wxStreamBuf* buf = new wxStreamBuf(textCtrl);

    // Redirect std::cout and std::cerr
    std::cout.rdbuf(buf);
    std::cerr.rdbuf(buf);
}
