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

string filename1 = "ListOfTutors_F.txt";
string filename2 = "KeyList_F.txt";

void add_tutor(const string& fname,
    const string& lname,
    const string& subject,
    const string& price,
    const string& city,
    const string& country,
    const string& email,
    const string& bio) {

    double position;
    fstream tutors(filename1);
    tutors.seekp(0,fstream::end);
    cout<<"Fuction running"<<endl;
    if (tutors.is_open()) {
        cout<<"tutors open"<<endl;
        position = tutors.tellp();
        cout<<position<<endl;
        tutors<<position<<"|"<<fname<<"|"<<lname<<"|"<<subject<<"|"<<price<<"|"<<"*0 ^0     ^"<<"|"<<city<<"|"<<country<<"|"<<email<<"|"<<bio<<"|"<<endl;
    }
    else {
        cout<<"ListOfTutors_F.txt could not be opened"<<endl;
        return;
    }
    tutors.close();
    fstream Keys(filename2);
    Keys.seekp(0,fstream::end);
    if (Keys.is_open()) {
        cout<<"Keys open"<<endl;
        Keys<<position<<"|"<<subject<<"|"<<price<<"|"<<"0"<<"|"<<endl;
    }
    else {
        cout<<"KeyList_F.txt could not be opened"<<endl;
        return;
    }
    Keys.close();
}

vector<string> line_to_vector(string input, vector<string> vec_interm){
    int pos1 = 0;
    char cur;
    string temp_str;
    
    for(int i = 0; i < input.size(); i++ ){
        cur = input[i];
        if (cur == '|'){
            temp_str = input.substr(pos1,i - pos1);
            vec_interm.push_back(temp_str);
            pos1 = i + 1;
        }
    }
    return vec_interm;
}

queue<vector<string>> SearchKeyFile(
    queue<vector<string>> Results,
    string key1)
{
    fstream Keys(filename2);
    string test;
    vector<string> temp_vector;


    if (Keys.is_open()){
        while(!Keys.eof())
        {
            getline(Keys,test);
            if(test.find(key1) != string::npos){
                temp_vector = line_to_vector(test,temp_vector);
                Results.push(temp_vector);
                temp_vector.clear();
            }
        }
    }
    else{
        cout<<"KeyList_F.txt could not be opened Search function"<<endl;
        return Results;
    }
    Keys.close();
    return Results;
}

void printV(vector<string> input){
    for (int i = 0; i < input.size(); i++)
    {
        cout << input[i] << " ";
    }
    cout<<endl;
}
void printQ(queue<vector<string>> input){
    while(!input.empty()){
        printV(input.front());
        input.pop();
    }
}

void change_rating(double location, string newRate)
{
    
}
