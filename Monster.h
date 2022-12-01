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

class Monster
{
    public:
        // DEFAULT CONSTRUCTOR
        Monster(); 

        // PARAMERTERIZED CONSTRUCTOR
        Monster(string monster, int level);

        // SETTERS 
        void setMonster(string monster);
        void setLevel(int level); 

        // SETTERS
        string getMonName();
        int getLevel();

    private:
        string monster_;
        int level_;
};

#endif