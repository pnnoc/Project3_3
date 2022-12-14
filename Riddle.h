// Riddle.h
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao

#ifndef RIDDLE_H
#define RIDDLE_H
#include <iostream>

using namespace std;

class Riddle
{ 
    public: 
        // DEFAULT CONSTRUCTOR 
        Riddle(); 

        // PARAMERTERIZED CONSTRUCTOR
        Riddle(string question, string answer);

        // GETTERS 
        string getQuestion();
        string getAnswer();

        // SETTERS
        void setQuestion(string question);
        void setAnswer(string answer);

    private:
        string answer_;
        string question_;

};

#endif


