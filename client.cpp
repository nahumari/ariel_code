#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <set>
#include <vector>
#include <time.h>

#define BUFSIZE 2048
 
using namespace std;

static stringstream output;

class ProcessMessage
{
public:

    ProcessMessage()
    {

        hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"), 
                        GENERIC_READ | GENERIC_WRITE, 
                        0,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);
        
        

    }
    void send(string str){
        
        if (hPipe != INVALID_HANDLE_VALUE)
        {
            WriteFile(hPipe,
                      str.c_str(),
                      str.length(),   // = length of string + terminating '\0' !!!
                      &dwWritten,
                      NULL);
    
        }
    }
    ~ProcessMessage()
    {
        if (hPipe)
            CloseHandle(hPipe);
    }
    
private:
    HANDLE hPipe;
    DWORD dwWritten;

};

bool is_odd(const int& num){
    return num % 2 != 0;
}
bool starts_with_1(const int& num){
    string str_num = to_string(num);
    return str_num[0] == '1';
}
bool has_3_digits(const int& num){
    string str_num = to_string(num);
    return str_num.length() == 3;
}
void handle_starts_with_1(const int& num){
    set<int> ordered_set;
    output << "[ origin list ";
    for (int i = 0 ; i < num ; i++){
        int new_num = max(1,rand()) % 1000;
        output << new_num << " ";
        ordered_set.insert(new_num);
    }
    output << "Ordered list: ";
    for (const int& val: ordered_set){
        output << val << " ";
    }
    output << "]" << endl;


}
int handle_3_digits(const int& num){
    string str_num = to_string(num);
    int mul = 1;
    for (int i = 0 ; i < str_num.length(); i++){
        mul *= (str_num[i] - '0');
    }
    return mul;

}
int main(int argc, char** argv)
{
    if (argc < 1){
        cerr << "error num of args "<< argc << endl;
        return 1;
    }

    int num_of_nums = std::atoi(argv[1]);

    srand(time(NULL));
    vector<int> initial_nums;
    output << "The selected random numbers are: ";
    for (int i = 0 ; i < num_of_nums ; i++){
        int curr_num = max(1,rand())  % 1000;
        initial_nums.push_back(curr_num);
        output << curr_num <<" ";
    }
    output << endl;
    for (auto& curr_num : initial_nums){

        if (is_odd(curr_num)){
            output << curr_num << ": this is an odd number " << endl;
        }
        else if (starts_with_1(curr_num)){
            output << curr_num << ": starts with 1" << endl;
            handle_starts_with_1(curr_num);
        }
        else if (has_3_digits(curr_num)){
            int mul = handle_3_digits(curr_num);
            output << curr_num <<": the digits multiplications are equal to " << mul << endl;
        }
        else{
            output << curr_num << ": this number belongs to the other" << endl;
        }
    }
    
    string str = output.str();
    ProcessMessage p;
    p.send(str);
 

    return (0);
}


