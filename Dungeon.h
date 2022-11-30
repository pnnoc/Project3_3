// Dunegeon.h
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao

#ifndef DUNGEON_H
#define DUNGEON_H
#include "Player.h"
#include "Party.h"
#include "Monster.h"
#include "Map.h"
#include "Riddle.h"

#include <iostream>

using namespace std;

class Dungeon
{
    public:
    
        Dungeon();
        Dungeon(Party party); //must initialize party before creating the dungeon class 
        //Dungeon(Party group, vector<Monster> mons);
        //Dungeon(Party group, vector<Monster> mons, vector<Riddle> riddles, Map map); //not sure
        void start(); //creating the map, placing rooms, NPC, etc + buying stuff


        //(vector<player> members, Party party, vector<Mons>, vector<riddle>, status[4], map, num player)

        //Players
        void addPlayer(Player player);
        void removePlayer();
        void removeWeapon(); //altogether with remove player();
        void removeArmor();
        int getNumPlayer();
        void setNumPlayer();
        Player getPlayerAt(int i);
        void setFullnessAt(int player_index, int new_fullness);
        int getFullnessAt(int player_index);

        //party
        void setPartyCookwareAt(int index, int value);
        void setPartyCookwarePriceAt(int index, int value);
        void setPartyWeaponAt(int index, int value);
        void setPartyWeaponPriceAt(int index, int value);
        void setPartyTreasureAt(int index, int value);
        void setPartyTreasurePriceAt(int index, int value);
        void setPartyIngredient(int value); 
        void setPartyIngredientPrice(int value);
        void setPartyArmor(int value);
        void setPartyArmorPrice(int value);
        void setPartyGold(int value);

        int getPartyCookwareAt(int index);
        int getPartyCookwarePriceAt(int index);
        int getPartyWeaponAt(int index);
        int getPartyWeaponPriceAt(int index);
        int getPartyTreasureAt(int index);
        int getPartyTreasurePriceAt(int index);
        int getPartyIngredient();
        int getPartyIngredientPrice();
        int getPartyArmor();
        int getPartyArmorPrice();
        int getPartyGold();
        int getNumWeapon();
        int getNumCookware();

        //Monster
        void readMonster(string filename); //reading monsters and levels, putting them into the vector
        Monster getMonsterAt(int i);
        void removeMonsterAt(int i);
        int getNumMonster();
        void setNumMonster();
        int getNumDefeatedMonster();
        void setNumDefeatedMonster(int num_mons_defeated);
        bool ishaveMonLevel(int mon_level);

        //Riddles
        void readRiddle(string filename); //reading riddles and answers, putting them into the vector
        Riddle getRiddleAt(int i);
        int getNumRiddle();
        void setNumRiddle();
        void removeRiddleAt(int i);

        //status
        int getStatusAt(int index);
        void setStatusAt(int index, int value);

        //Map
        Map getMap();
        //void dun_setPlayerFullness(int index, int fullness);

        //giveup
        bool getGiveup();
        void setGiveup(bool giveup);



        //other functions
        bool isInputInteger(string);
        bool probability(int prob_percent); //to calculate whether the action is yes/no true/false
        bool yesorno(string yn); //for confirming buying
        bool isEndgame(); //including all the condition to check whether the game will end or not. used after each action or turn
        void inventoryUpdate();
        void partyUpdate();
        void statusUpdate(); //whole displat for each move, including inventory and party update
        void moveAction();
        void fightingMonster();
        void giveUp();
        bool hungerWarning();
        int getNumExplored();
        void fightingSorcerer(); //for extra credit
        int attackingScore();

        void mainNormalSpace();
        void printNormalSpaceActions(); //options when entering new space
        void normalSpaceInvestigate();
        void normalSpacePickfight(); //just fightingMonster+50%chance condition
        void normalSpaceCookeat();

        void mainMerchant();
        void printMerchant(); //types of item you wanna buy
        void merchant_ingredient();
        void merchant_cookware();
        void merchant_weapon();
        void merchant_armor();
        void merchant_sell(); // all these 4 sub function might be able to just code in the MERCHANT without having to define separately, but might not be a good idea in while loop before player choose to leave

        void mainNPCAction();
        void printNPCAction();
        void npc_speak();

        void mainRoom();
        void printRoomActions();
        //void room_move(); //share movefunction
        void roomOpen();
        //void room_giveup(); //probably dont need this

        void misfortuneRoom(); //40% chance to happen
        //there is also probability in the misfortune
        //gonna be used after the SpaceAction and RoomAction
        void misfortuneNormal();

        //Note! maybe having the function that includes sub level menu. For example,  SPACE = printAction -> sub choices action1-5. or MERCHANT = printMerchant -> sub items player wanna buy

        void mainGame();
        void endGame();
        void saveStats();
        void saveScore();
        double scoreCalculation();
        void scoreBoard();


    private:
        int num_player_;
        vector<Player> members_;

        Party party_; //cookware_ array, treasure_ array, gold_, ingredient_, armor_

        int num_mons_defeated_;
        int num_mons_;
        vector<Monster> mons_;

        int num_riddles_; 
        vector<Riddle> riddles_;

        Map map_;
        int status_[4]; // {room cleared, keys, anger level, turns}
        bool giveup_;
        //vector of num_turn in each game to be sorted later***

        //Note! modifying each element by using their own class functions
};
#endif

