#ifndef APP_H
#define APP_H
#include <wx/wx.h>
#include "MainFrame.h"

//Header file for app

class App : public wxApp {
public:
    virtual bool OnInit() override {
        MainFrame* frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};


#endif
