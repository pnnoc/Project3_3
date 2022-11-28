// Monster.cpp
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao

#include <iostream>
#include "Monster.h" 
using namespace std;
 
Monster::Monster()
{
    monster_ = "";
    level_ = 0;
} 

Monster::Monster(string monster, int level)
{
    monster_ = monster;
    level_ = level;
}

void Monster::setMonster(string monster)
{
    monster_ = monster;
}
void Monster::setLevel(int level)
{
    level_ = level;
}
string Monster::getMonName()
{
    return monster_;
}
int Monster::getLevel()
{
    return level_;
}


