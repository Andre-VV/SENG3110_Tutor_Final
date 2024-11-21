#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
using namespace std;

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

int main(){
    vector<string> test_data_vec;
    string test_data = "116|Math|40.00|0|";

    test_data_vec = line_to_vector(test_data,test_data_vec);

    for (int i = 0; i < test_data_vec.size(); i++)
    {
        cout << test_data_vec[i] << " ";
    }
    return 0;
}