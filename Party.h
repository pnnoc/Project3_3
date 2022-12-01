// Party.h
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao

#ifndef PARTY_H
#define PARTY_H 

#include "Player.h" 

#include <iostream>
#include <vector> 

using namespace std;

class Party
{
    public:
        // DEFAULT CONSTRUCTOR
        Party();

        // SETTERS
        void setGold(int gold); 
        void setIngredient(int ingredient);
        void setIngredientPrice(int price); 
        void setArmor(int armor);
        void setArmorPrice(int price); 
        void setTreasureAt(int index, int treasure);
        void setTreasurePriceAt(int index, int price);
        void setCookwareAt(int index, int cookware);
        void setCookwarePriceAt(int index, int price);
        void setWeaponAt(int index, int weapon);
        void setWeaponPriceAt(int index, int price);

        // GETTERS
        int getArmor();
        int getArmorPrice();
        int getIngredient();
        int getIngredientPrice();
        int getGold();
        int getTreasureAt(int i);
        int getTreasurePriceAt(int i);
        int getCookwareAt(int i); 
        int getCookwarePriceAt(int i); 
        int getWeaponAt(int i);
        int getWeaponPriceAt(int i);

    private:
        int gold_;
        int ingredient_[2]; //array of two containing amount and price
        int armor_[2];
        //cookware: 3 cookwares in order (p=pot, f=pan, c=cauldron )
        static const int COOK_SIZE=3;
        int cookware_[COOK_SIZE][2];
        //Treasures: 5 treasures in order (R=ring, N=necklace, B=bracelet, C=circlet, G=goblet)
        static const int TREA_SIZE=5;
        int treasure_[TREA_SIZE][2];
        static const int WEAP_SIZE=5;
        int weapon_[WEAP_SIZE][2];
        //int num_player_;
        //vector<Player> members_; dont need
};

#endif