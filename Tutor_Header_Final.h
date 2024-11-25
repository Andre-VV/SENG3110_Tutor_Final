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
#include <list>

using namespace std;


//file names to use in functions
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


    //Adds a entry to main data file and an entry to the key file
    double position;
    fstream tutors(filename1);
    tutors.seekp(0,fstream::end);
    cout<<"Fuction running"<<endl;
    if (tutors.is_open()) {
        //cout<<"tutors open"<<endl;
        position = tutors.tellp();
        //cout<<position<<endl;
        tutors<<position<<"|"<<fname<<"|"<<lname<<"|"<<subject<<"|"<<price<<"|"<<"*0.0 ^0.0000^"<<"|"<<city<<"|"<<country<<"|"<<email<<"|"<<bio<<"|"<<endl;
    }
    else {
        cout<<"ListOfTutors_F.txt could not be opened"<<endl;
        return;
    }
    tutors.close();
    fstream Keys(filename2);
    Keys.seekp(0,fstream::end);
    if (Keys.is_open()) {
        //cout<<"Keys open"<<endl;
        Keys<<position<<"|"<<subject<<"|"<<price<<"|"<<"0.0"<<"|"<<endl;
    }
    else {
        cout<<"KeyList_F.txt could not be opened"<<endl;
        return;
    }
    Keys.close();
}

vector<string> line_to_vector(string input, vector<string> vec_interm){
    //converts a string from a file to a vector using "|" as a delimiter
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

list<vector<string>> listSort(list<vector<string>> input_list, vector<string> input_vec){
    //adds a value to a list in the correct location so the list is sorted
    vector<string> vector1;
    list<vector<string>>::iterator point;
    string rate1;
    string rate2;
    double rate1_d;
    double rate2_d;

    rate2 = input_vec[3];
    rate2_d = stod(rate2);

    for(point = input_list.begin(); point != input_list.end(); point++){
        vector1 = *point;

        rate1 = vector1[3];
        rate1_d = stod(rate1);

        if(rate2_d > rate1_d){
            input_list.insert(point,input_vec);
            return input_list;
        }
        vector1.clear();
    }
    input_list.push_back(input_vec);
    return input_list;

}

queue<vector<string>> sortQ(queue<vector<string>> input){
    //sorts a list by rating in decending order
    //takes an unsorted queue as an input and oupputs a sorted queue
    list<vector<string>> sortingList;
    vector<string> vector1;

    vector1 = input.front();
    input.pop();
    sortingList.push_front(vector1);
    vector1.clear();

    while(!input.empty()){
        vector1 = input.front();
        input.pop();
        sortingList = listSort(sortingList,vector1);
        vector1.clear();
    }
    long sortSize = sortingList.size();
    for(long i = 0; i < sortSize; i++){
        input.push(sortingList.front());
        sortingList.pop_front();
    }
    return input;
}

queue<vector<string>> SearchKeyFile(
    queue<vector<string>> Results,
    string key1)
{
    //searches the key file for lines with a key in them and adds them to a queue
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
    Results = sortQ(Results);
    return Results;
}

void printV(vector<string> input){
    //prints vector to the teminal
    for (int i = 0; i < input.size(); i++)
    {
        cout << input[i] << " ";
    }
    cout<<endl;
}
void printQ(queue<vector<string>> input){
    //using printV prints a queue to the terminal
    while(!input.empty()){
        printV(input.front());
        input.pop();
    }
}


vector<string> GetTutorLine(double location){
    //gets a line from the main data file based on location
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
    //gets the rating or the number of rating from a line in the main data file
    //based on location
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
            //cout<<current_R<<endl;
            tutors.close();
            //cout<<endl<<"Found rating: "<<current_R<<endl;
            return current_R;   
        }
        else{
            for(int i = 6; i < rate_Section.size(); i++){
                if(rate_Section[i] == '.'){
                    num_rates_str = rate_Section.substr(6,i-4);
                    break; 
                }
            }
            num_rates = stod(num_rates_str);
            //cout<<num_rates<<endl;
            tutors.close();
            //cout<<endl<<"Found number of Ratings: "<<num_rates;
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
    //inserts a rating and number of ratings into a line in the main data file
    //also changes rating in the keyfile

    fstream tutors(filename1);
    vector<string> interm;
    string rate_Section;
    char search;
    string rate_str;
    string num_str;
    char const *rate_char;
    char const *num_char;
    double position = 0;
    double error;
    double calc;


    if (tutors.is_open()){
        tutors.seekg(0);
        tutors.seekp(0);
        tutors.seekg(location);
        tutors.seekp(location);
        //cout<<endl<<endl;
        search = tutors.get();
        calc = tutors.tellg();
        error = (calc - 1) - location;
        //cout<<"error: "<<error<<endl;

        while(search != '*'){
            search = tutors.get();
            //cout<<search<<tutors.tellg()<<endl;

        }
        calc = tutors.tellg();
        tutors.seekp(calc - error);


        rate_str = to_string(Rate);
        //cout<<endl<<"rate_str: "<<rate_str<<endl;
        rate_str.resize(3);
        //cout<<"rate_str: "<<rate_str<<endl;
        rate_char = rate_str.c_str();
        //cout<<"rate_char: "<<rate_char<<endl;

        num_str = to_string(num_rate);
        //cout<<endl<<"num_str: "<<num_str<<endl;
        num_str.resize(6);
        //cout<<"num_str: "<<num_str<<endl;
        num_char = num_str.c_str();
        //cout<<"num_char: "<<num_char<<endl;
        tutors.write(rate_char,3);
        position = tutors.tellp();
        tutors.seekp(position+2);
        tutors.write(num_char,6);
    }
    else{
        cout<<"ListOfTutors_F.txt could not be opened. insert_New_Rating"<<endl;
        return;
    }
    tutors.close();
    fstream keys(filename2);
    vector<string> keyLine;
    string locationString;
    string tempLine;
    double locationKeys;
    string temp_str;
    double errorLocation;
    int errorKeys1;
    int errorKeys2;
    long errorCount = 1;
    
    

    if(keys.is_open()){
        //cout<<endl<<"keyfile part:"<<endl<<"location double: "<<location<<endl;
        locationString = to_string((long)location);
        //cout<<"Location string: "<<locationString<<endl;
        keys.seekp(0);
        //cout<<"location: "<<keys.tellp()<<endl;
        getline(keys,tempLine);
        errorKeys2 = keys.tellp();
        //cout<<"start of line 2: "<<errorKeys2<<endl;
        errorKeys1 = tempLine.size();
        //cout<<"size of line 1: "<<errorKeys1<<endl;

        keys.seekp(0);
        error = errorKeys2 - errorKeys1;
        //cout<<"error: "<<error<<endl;

        

        while(!keys.eof()){
            locationKeys = keys.tellg();
            //cout<<"searching: "<<locationKeys<<endl;
            getline(keys,tempLine);
            keyLine = line_to_vector(tempLine,keyLine);
            if(keyLine[0] == locationString){;
                //cout<<"Found: "<<keys.tellg()<<"  Write location: "<<keys.tellp()<<endl;
                keyLine[3] = rate_str;
                keys.seekp(0);
                //cout<<"Return to begining: "<<keys.tellp()<<endl;
                locationKeys = locationKeys - (error - errorCount);
                if (locationKeys < 0){
                    locationKeys = 0;
                }
                keys.seekp(locationKeys);
                //cout<<"write Location: "<<keys.tellp()<<endl;
                for(int i = 0; i < 4; i++){
                    temp_str = keyLine[i];
                    //cout<<"inserting: "<<temp_str<<"  Location: "<<keys.tellp()<<endl;
                    keys.write(temp_str.c_str(),temp_str.size());
                    locationKeys = keys.tellp();
                    keys.seekp(locationKeys+1);
                    //cout<<"end location: "<<keys.tellp()<<endl;

                }
                keys.close();
                return;

            }
            errorCount++;
            keyLine.clear();
            
        }

    }
    else{
        cout<<"KeyLift_F.txt could not be opened. insert_New_Rating"<<endl;
        return;
    }   
}



void Rate_Tutor(double location, double new_rate){
    //main function to rate a tutor.
    double cur_rate;
    double num_rate;
    double new_num_rate;
    float calc;

    cur_rate = getRating(location,true);
    num_rate = getRating(location,false);

    calc = (cur_rate * num_rate) + new_rate;
    new_num_rate = num_rate + 1;
    calc = calc/(new_num_rate);
    //cout<<endl<<calc<<endl;
    calc = roundf(calc *10)/10;
    //cout<<calc<<endl;

    insert_New_Rating(location,calc,new_num_rate);
}




