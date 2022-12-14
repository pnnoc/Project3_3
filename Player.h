// Player.h
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

using namespace std; 

class Player 
{
    public:
        // Player(); // not neccessary 
        // PARAMERTERIZED CONSTRUCTOR
        Player(string name); 
        
        // GETTERS
        string getName();
        int getFullness();

        // SETTERS
        void setName(string);
        void setFullness(int fullness); //dereasing after turns and fighting //what should be the argument

    private:
        string name_;
        int fullness_;
};

#endif