#include <iostream>
#include <fstream>
#include <vector>
#include <string>
// #include "Monster.h"
// #include "Dungeon.h"
// #include "Player.h"
// #include "Party.h"

using namespace std;

// void change(vector<int> vec)
// {
//     vec.at(0) = 13;
//     vec.at(vec.size()-1) = 10; 
// }

// Monster split_monster(string input_string, char separator) //only for monster, so needed to be adjusted
// {
//     int num_monster=0;
//     int instring_len = input_string.length();
//     Monster read_mon;
//     for (int i=0; i < instring_len ; i++) //going through each char of strinng
//     {
//         if (input_string[i] == separator) //when separator is found
//         {
//             string mon_name = input_string.substr(0,i); //before the , will be put into mon name which is string
//             int mon_level = stoi(input_string.substr(i));
//             read_mon.setMonster(mon_name);
//             read_mon.setLevel(mon_level);
//         }
//     }
//     return read_mon; //gonna be only one becasue of 1 line
// }

// void Dungeon::readMonster(string filename) //reading file, slpit names and levels, and put them into the vector of monsters
// {
//     ifstream fin;
//     fin.open(filename);
//     string line;
//     char separator = ',';
//     while (getline(fin, line))
//     {
//         Monster read_mon = split_monster(line, separator);
//         mons_.push_back(read_mon);
//     }
//     return;
// }

int main()
{
    // Party group;
    // vector<Monster> mon;
    // Dungeon game_on(group, mon);
    // string filename;
    // cout << "Enter you monster filename: " << endl;
    // cin >> filename;
    // game_on.readMonster(filename);

    // vector<int> vec = {0,1,2,3,4,5,6,7,8};
    // //vector<int> vec3 = vec;
    // // for (int i=0; i<vec3.size(); i++)
    // // {
    // //     cout << vec3.at(i) << " ";
    // // }
    // // cout << endl;
    // vector<int> &vec2 = vec;
    // change(vec);
    // cout << vec.at(0) << endl;
    // cout << vec.at(vec.size()-1) << endl;

    // cout << vec2.at(0) << endl;
    // cout << vec2.at(vec.size()-1) << endl;

    // int i=10;
    // int &r = i;

    // r=9;
    // cout << r << endl;


    // int ori_price = 99; 
    // int n;
    // cin >> n;

    // int new_price = ori_price*(1+(n*0.25));
    // // if (n==1)
    // // {
    // //     int increase = ori_price*1.25;
    // //     cout << increase << endl;
    // // }
    // // esle if (n==2)
    // // {
    // //     int increase = ori_price*1.50
    // // } 
    // //int result = 99/10; 
    // //cout << result << endl;
    // cout << new_price << endl;

    // int price = 33;
    // int result = price*0.75;
    // cout << result << endl;
    // string cw;
    // cin >> cw;
    // while (!(cw=="1" || cw=="2" || cw=="3"))
    // {
    //     cout << "Please enter 1, 2, or 3." << endl;
    //     cin >> cw;
    // }


    // vector<vector<string>> name_score;
    // vector<string> temp_array(2); //must giving the size, if not it will give error of segmentation fault
    // ifstream fin;
    // string filename = "score.txt";
    // fin.open(filename);
    // string line;
    // int count =0;
    // char separater = ':';
    // while(getline(fin, line))
    // {
    //     if (line.length()!=0)
    //     {
    //         cout << line << endl;
    //         count++;
    //         for (int i=0; i<line.length(); i++)
    //         {
    //             if (line[i]==separater)
    //             {
    //                 // cout << "Found" << endl;
    //                 // cout << i << endl;
    //                 // cout << line.substr(0,i) << endl; //passed
    //                 // cout << line.substr(i+1) << endl; //passed

    //                 temp_array.at(0)=line.substr(0,i);
    //                 temp_array.at(1)=line.substr(i+1);
    //                 break;
    //                 //temp_array[0]=line.substr(0,i); //starting at 0 and stop in front of i(separater) -> this line give segmentation fault
    //                 // temp_array[1]=line.substr(i+1); //cutting after the separator til the end of the line
    //             }
    //         }
    //         //cout << temp_array[0] << " " << temp_array[1] << endl;
    //     }
    // }
    // cout << count << endl;

    string input;
    cin >> input;
    while (input.length()==1 && stoi(input)) //possibly have to use ascii number
    {
        cout << "Input is not number." << endl;
        cin >> input;
    }
    return 0;
}