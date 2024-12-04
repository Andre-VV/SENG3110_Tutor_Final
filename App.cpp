#include <wx/wx.h>
#include "MainFrame.h" 
//Class App to declare the MainFrame and implement the app
class App : public wxApp {
public:
    virtual bool OnInit() {
        MainFrame* frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(App);  
