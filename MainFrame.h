#ifndef MAINFRAME_H
#define MAINFRAME_H
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
    MainFrame();
    // Function to redirect output to wxTextCtrl
    void RedirectOutputToTextCtrl(wxTextCtrl* textCtrl);
private:
    void OnSearchTutors(wxCommandEvent& event);
    void OnAddTutor(wxCommandEvent& event);
};

#endif
