#include<iostream>
#include<fstream>
#include<vector>
#include "Riddle.h"

using namespace std;

void readRiddle(vector<Riddle>& riddle_vec ,string filename)
{
    ifstream fin;
    fin.open(filename);
    string line;
    char separator = '~';
    while (getline(fin, line))
    {
        Riddle riddle_read;
        for (int i=0; i < line.length(); i++)
        {
            if (line[i] == separator)
            {
                riddle_read.setQuestion(line.substr(0,i));
                riddle_read.setAnswer(line.substr(i+1)); //i+1 becasue we dont want separator
                riddle_vec.push_back(riddle_read);
                break;;
            }
        }
    }
    fin.close();
    return;
}

void printRiddle(vector<Riddle> vec)
{
    for (int i=0; i<vec.size(); i++)
    {
        cout << vec.at(i).getQuestion() << endl;
        cout << vec.at(i).getAnswer() << endl;
    }
    return;
}

int main()
{
    Riddle riddle1;
    vector<Riddle> riddle_set;
    cout << riddle_set.size() << endl;
    readRiddle(riddle_set, "riddles.txt");
    cout << riddle_set.size() << endl; // =20
    printRiddle(riddle_set);
    return 0;
}