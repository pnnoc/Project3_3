// Party.cpp
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao

#include <iostream>
#include <vector>
#include "Player.h"
#include "Party.h"

using namespace std;

Party::Party()
{
    //vector<Player> members_; //setting an empty vector of player
    for (int i=0; i<COOK_SIZE; i++)
    {
        cookware_[i][0] = 0;
        if (i==0)
        {
            cookware_[i][1] = 2; //initial price of pot
        }
        else if (i==1)
        {
            cookware_[i][1] = 10; //initial price of pan
        }
        else if (i==2)
        {
            cookware_[i][1] = 20; //initial price of cauldron
        }
    }
    for (int i=0; i<TREA_SIZE; i++)
    {
        treasure_[i][0] = 0;
        if (i==0)
        {
            treasure_[i][1] = 10; //initial selling price of ring
        }
        else if (i==1)
        {
            treasure_[i][1] = 20; //initial selling price of necklace
        }
        else if (i==2)
        {
            treasure_[i][1] = 30; //initial selling price of bracelet
        }
        else if (i==3)
        {
            treasure_[i][1] = 40; //initial selling price of circlet
        }
        else if (i==4)
        {
            treasure_[i][1] = 50; //initial selling price of goblet
        }
    }
    for (int i=0; i<WEAP_SIZE; i++)
    {
        weapon_[i][0] = 0;
        if (i==0)
        {
            weapon_[i][1] = 2; //initial selling price of ring
        }
        else if (i==1)
        {
            weapon_[i][1] = 2; //initial selling price of necklace
        }
        else if (i==2)
        {
            weapon_[i][1] = 5; //initial selling price of bracelet
        }
        else if (i==3)
        {
            weapon_[i][1] = 15; //initial selling price of circlet
        }
        else if (i==4)
        {
            weapon_[i][1] = 50; //initial selling price of goblet
        }
    }
    gold_ = 100;
    ingredient_[0] = 0;
    ingredient_[1] = 1; //initial prize of ingredient
    armor_[0] = 0;
    armor_[1] = 5; //initial price of suit armor
    //num_player_=0;
}
void Party::setGold(int gold) //passed
{
    gold_ = gold;
}
void Party::setArmor(int armor) //passed
{
    armor_[0] = armor;
}
void Party::setArmorPrice(int price)
{
    armor_[1] = price;
}
void Party::setIngredient(int ingredient) //passed
{
    ingredient_[0] = ingredient;
}
void Party::setIngredientPrice(int price)
{
    ingredient_[1] = price;
}
void Party::setTreasureAt(int i, int treasure) //passed
{
    treasure_[i][0] = treasure;
}
void Party::setTreasurePriceAt(int i, int price)
{
    treasure_[i][1] = price;
}
void Party::setCookwareAt(int i, int cookware) //passed
{
    cookware_[i][0] = cookware;
}
void Party::setCookwarePriceAt(int i, int price)
{
    cookware_[i][1] = price;
}
void Party::setWeaponAt(int i, int weapon) //passed
{
    weapon_[i][0] = weapon; 
}
void Party::setWeaponPriceAt(int i, int price)
{
    weapon_[i][1] = price;
}


int Party::getArmor()
{
    return armor_[0];
}
int Party::getArmorPrice()
{
    return armor_[1];
}
int Party::getGold()
{
    return gold_;
}
int Party::getIngredient()
{
    return ingredient_[0];
}
int Party::getIngredientPrice()
{
    return ingredient_[1];
}
int Party::getTreasureAt(int i)
{
    return treasure_[i][0];
}
int Party::getTreasurePriceAt(int i)
{
    return treasure_[i][1];
}
int Party::getCookwareAt(int i)
{
    return cookware_[i][0];
}
int Party::getCookwarePriceAt(int i)
{
    return cookware_[i][1];
}
int Party::getWeaponAt(int i)
{
    return weapon_[i][0];
}
int Party::getWeaponPriceAt(int i)
{
    return weapon_[i][1];
}
