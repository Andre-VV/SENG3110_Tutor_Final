//
// Created by Owner on 11/21/2024.
//

#ifndef TUTOR_MAIN_FINAL_H
#define TUTOR_MAIN_FINAL_H

#endif //TUTOR_MAIN_FINAL_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;


void add_tutor(const string& fname,
    const string& lname,
    const string& subject,
    const string& price,
    const string& city,
    const string& country,
    const string& email,
    const string& bio) {

    double position;
    fstream tutors("ListOfTutors_F.txt", ios::ate);
    cout<<"Fuction running"<<endl;
    if (tutors.is_open()) {
        cout<<"tutors open"<<endl;
        position = tutors.tellp();
        tutors<<position<<" "<<fname<<" "<<lname<<" "<<subject<<" "<<price<<" "<<"0"<<" "<<city<<" "<<country<<" "<<email<<" "<<bio<<endl;
    }
    else {
        cout<<"ListOfTutors_F.txt could not be opened"<<endl;
        return;
    }
    tutors.close();
    fstream Keys("KeyList_F.txt", ios::ate);
    if (Keys.is_open()) {
        cout<<"Keys open"<<endl;
        Keys<<position<<" "<<subject<<" "<<price<<" "<<"0"<<endl;
    }
    else {
        cout<<"KeyList_F.txt could not be opened"<<endl;
        return;
    }
    Keys.close();
}

void find_tutor(){

}