// Riddle.cpp
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao

#include <iostream>
#include "Riddle.h"

using namespace std;

Riddle::Riddle()
{
    question_ = "";
    answer_ = "";
}

Riddle::Riddle(string question, string answer)
{
    question_ = question;
    answer_ = answer;
}
void Riddle::setQuestion(string question)
{
    question_ = question;
}
void Riddle::setAnswer(string answer)
{
    answer_ = answer;
}
string Riddle::getAnswer()
{
    return answer_;
}
string Riddle::getQuestion()
{
    return question_;
}
