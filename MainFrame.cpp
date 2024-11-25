#include "MainFrame.h"
#include "SearchTutor.h"
#include "AddTutor.h"
//Creates the main boxes and buttons to be clicked upon
MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Tutor Management", wxDefaultPosition, wxSize(400, 300)) {
    wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);

    wxButton* searchButton = new wxButton(this, wxID_ANY, "Search for Tutors");
    wxButton* addButton = new wxButton(this, wxID_ANY, "Add a Tutor");

    layout->Add(searchButton, 0, wxALL | wxEXPAND, 10);
    layout->Add(addButton, 0, wxALL | wxEXPAND, 10);

    searchButton->Bind(wxEVT_BUTTON, &MainFrame::OnSearchTutors, this);
    addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddTutor, this);

    SetSizer(layout);
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
