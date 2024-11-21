//
// Created by Owner on 11/21/2024.
//

#include "Tutor_Header_Final.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>

queue<vector<string>> internal_Queue;

int main(){
  

  add_tutor("Andrew","Toderian","Science","50.00",
    "Kamloops","Canada","email","this is my bio it is very long and a good way to test this");
  add_tutor("Owen","Toderian","Math","40.00",
    "Kamloops","Canada","email","this is my bio it is very long and a good way to test this");
  
  cout<<endl;
  internal_Queue = SearchKeyFile(internal_Queue,"Math");
  printQ(internal_Queue);
  cout<<endl<<endl;




  
  return 0;
}