#ifndef MAINFRAME_H
#define MAINFRAME_H
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    void OnSearchTutors(wxCommandEvent& event);
    void OnAddTutor(wxCommandEvent& event);
};

#endif
