// Player.cpp
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao
#include <iostream>
#include "Player.h"
using namespace std;

Player::Player(string name)
{
    name_ = name; 
    fullness_ = 50;
}

string Player::getName()
{
    return name_;
}

int Player::getFullness()
{
    return fullness_;
}

void Player::setName(string name)
{
    name_ = name;
}

void Player::setFullness(int fullness)
{
    fullness_ = fullness;
}
