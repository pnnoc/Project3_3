#include <iostream>
#include "Monster.h" 
#include <fstream>
#include <vector>

using namespace std;

void monPrint(Monster mon)
{ 
    cout << mon.getMonName() << endl;
    cout << mon.getLevel() << endl;
    
    return;
}

void readMonster(string filename)
{
    ifstream fin;
    fin.open(filename);
    string line;
    while (getline(fin, line))
    {
        Monster mon_read;
        mon_read.setMonster(line.substr(0,line.length()-2));
        mon_read.setLevel(int(line[line.length()-1])-48); //-48 becasue of ascii
        monPrint(mon_read);
    }
    return;
}
int main()
{
    Monster mon1;
    monPrint(mon1);
    cout << endl;
    mon1.setLevel(2);
    mon1.setMonster("Zack");
    monPrint(mon1);


    vector<Monster> vec_mon;
    string filename;
    cout << "Enter filename: ";
    cin >> filename;
    
    readMonster(filename);
    

    
    return 0;
}