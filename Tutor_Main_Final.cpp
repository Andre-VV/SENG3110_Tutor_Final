//
// Created by Andrew on 11/21/2024.
//

#include "Tutor_Header_Final.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>


queue<vector<string>> internal_Queue;

int main(){
  
  /*
  add_tutor("John","Smith","Science","50.00",
    "Kamloops","Canada","email","this is my bio it is very long and a good way to test this JS");
  add_tutor("Andrew","Toderian","Math","90.00",
    "Kamloops","Canada","email","this is my bio it is very long and a good way to test this AT");
  add_tutor("Joe","Mama","Science","900.00",
    "Kamloops","Canada","email","this is my bio it is very long and a good way to test this JM");
  add_tutor("Tony","Hawk","Science","40.00",
    "Kamloops","Canada","email","this is my bio it is very long and a good way to test this TH");
  add_tutor("Owen","Toderian","Science","50.00",
    "Kamloops","Canada","email","this is my bio it is very long and a good way to test this JM");
  add_tutor("Luke","Skywalker","Math","90.00",
    "Kamloops","Canada","email","this is my bio it is very long and a good way to test this JMs");
  */

  Rate_Tutor(0,5.0);
  Rate_Tutor(127,3.0);
  Rate_Tutor(258,4.5);
  Rate_Tutor(386,1.3);
  Rate_Tutor(514,0.2);
  Rate_Tutor(646,5.0);
  
  return 0;
}