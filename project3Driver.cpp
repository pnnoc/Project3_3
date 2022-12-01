// Project3Driver.cpp
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao

#include <iostream> 
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <string>
#include "Player.h"
#include "Party.h"
#include "Riddle.h"
#include "Monster.h" 
#include "Dungeon.h"

using namespace std;

int main()
{
    srand(time(0));
    Dungeon game1;
    game1.start();
    game1.mainGame();
    game1.endGame();
    game1.saveStats();
    game1.saveScore();
    game1.scoreBoard();

    

     
    return 0;
}