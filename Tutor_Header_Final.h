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
#include <math.h>

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
        tutors<<position<<"|"<<fname<<"|"<<lname<<"|"<<subject<<"|"<<price<<"|"<<"*0.0 ^0     ^"<<"|"<<city<<"|"<<country<<"|"<<email<<"|"<<bio<<"|"<<endl;
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
        Keys<<position<<"|"<<subject<<"|"<<price<<"|"<<"0.0"<<"|"<<endl;
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


vector<string> GetTutorLine(double location){
    fstream tutors(filename1);
    string line;
    vector<string> interm;

    if (tutors.is_open()) {
        tutors.seekg(location);
        getline(tutors,line);
        interm = line_to_vector(line,interm);
    }
    else {
        cout<<"ListOfTutors_F.txt could not be opened. GettutorLine"<<endl;
        return interm;
    }
    tutors.close();
    return interm;
}

double getRating(double location, bool R_or_num){
    //True returns rating. False returns number of ratings
    fstream tutors(filename1);
    vector<string> interm;
    string rate_Section;
    string cur_R_str;
    string num_rates_str;
    double current_R;
    double num_rates;

    if (tutors.is_open()) {
        interm = GetTutorLine(location);
        rate_Section = interm[5];
        if (R_or_num == true){
            cur_R_str = rate_Section.substr(1,3);
            current_R = stod(cur_R_str);
            cout<<current_R<<endl;
            tutors.close();
            return current_R;   
        }
        else{
            for(int i = 6; i < rate_Section.size(); i++){
                if(rate_Section[i] == ' '){
                    num_rates_str = rate_Section.substr(6,i-4);
                    break; 
                }
            }
            num_rates = stod(num_rates_str);
            cout<<num_rates<<endl;
            tutors.close();
            return num_rates;
        }
    }
    else {
        cout<<"ListOfTutors_F.txt could not be opened. getRating"<<endl;
        return 0;
    }
    tutors.close();
    return 0; 
}
void insert_New_Rating(double location, float Rate, double num_rate ){
    fstream tutors(filename1);
    vector<string> interm;
    string rate_Section;
    char search;
    if (tutors.is_open()){
        tutors.seekg(location);
        cout<<endl<<endl;
        while(search != '*'){
            search = tutors.get();
            cout<<search;

        }
    }
    else{
        cout<<"ListOfTutors_F.txt could not be opened. insert_New_Rating"<<endl;
        return;
    }
    tutors.close();


}

void Rate_Tutor(double location, double new_rate){
    double cur_rate;
    double num_rate;
    double new_num_rate;
    float calc;

    cur_rate = getRating(location,true);
    num_rate = getRating(location,false);

    calc = (cur_rate * num_rate) + new_rate;
    new_num_rate = num_rate + 1;
    calc = calc/(num_rate + 1);
    cout<<endl<<calc<<endl;
    calc = roundf(calc *10)/10;
    cout<<calc<<endl;


    
    

}
