#ifndef SEARCHTUTOR_H
#define SEARCHTUTOR_H

#include <wx/wx.h>


class SearchTutor : public wxDialog {
public:
    SearchTutor(wxWindow* parent);
    void DisplayTutorDetails(wxListBox* tutorList, int selectedIndex, wxChoice* filterDropdown);
    void RateTutor(wxListBox* tutorList, int selectedIndex, wxChoice* filterDropdown);
    void PopulateFilteredTutors(wxListBox* tutorList, const std::string& subject);
    void PopulateAllTutors(wxListBox* tutorList);
};

#endif 
