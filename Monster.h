// Monster.h
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao

#ifndef MONSTER_H 
#define MONSTER_H

#include <iostream> 

using namespace std;

// mayneed to use split function 

class Monster
{
    public:
        Monster(); //not necessary?
        Monster(string monster, int level);
        void setMonster(string monster);
        void setLevel(int level); 

        string getMonName();
        int getLevel();

    private:
        string monster_;
        int level_;
};

#endif