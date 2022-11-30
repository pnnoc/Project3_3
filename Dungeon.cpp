// Dunegeon.cpp
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao

#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <cctype> //to use toupper()
#include "Dungeon.h"

#include <ctime> //where do we have to add these three
#include <cstdlib>
#include <math.h>

using namespace std;

Dungeon::Dungeon()  //how to initialize
{
    members_; //does not seem to be problem
    party_; //indlucing party, cookware array, treasure array, gold, ingredient, armor
    mons_; //vector size 0 (note sure if it can be initialized in this way)
    riddles_; //vector size 0 (note sure if it can be initialized in this way)
    map_;
    for (int i=0; i< 4; i++)
    {
        status_[i] = 0;
    }
    num_player_=0;
    num_mons_=0;
    num_mons_defeated_=0;
    num_riddles_=0;
    giveup_=false;

}
// Dungeon:: Dungeon(Party party)
// {
//     party_ = party;
//     mons_;
//     map_;
//     riddles_;
//     for (int i=0; i< 4; i++)
//     {
//         status_[i] = 0;
//     }

// }
// Dungeon::Dungeon(Party party, vector<Monster> mons)
// {
//     party_ = party;
//     mons_ = mons;
//     map_;
//     riddles_;
//     turn_ = 0;
// }

//starting the game
void Dungeon::start() //done, tested
{
    map_.resetMap(); //setting X at the origin (player's position) + setting the Exit att the middle of the last row
    //adding rooms
    while (map_.getRoomCount() < map_.getMaxRooms()) // need to generalize 5 in case of wanting rooms to be more than 5
    {
        int row = rand()%(map_.getNumRows()-1); // randomize the number of row from 0-10 (we dont want the last row)
        int col = rand()%(map_.getNumCols()-1); // randomize the number of col from 0-10 (we dont want the last row)
        //cout << row << "," << col << endl;
        map_.addRoom(row,col); //checking freeSpace is already in this function also room_count++
    }
    //adding NPCs
    // cout << map_.getNPCCount() << endl;
    // cout << map_.getMaxNpcs() << endl;
    while (map_.getNPCCount() < 5) //map_.getMaxNpcs()
    {
        int row = rand()%(map_.getNumRows()-1); // randomize the number of row from 0-10 (we dont want the last row)
        int col = rand()%(map_.getNumCols()-1); // randomize the number of col from 0-10 (we dont want the last row)
        //cout << row << "," << col << endl;
        map_.addNPC(row,col); //checking freeSpace is already in this function also npc_count++.
    }
    map_.displayMap();

    cout << endl;
    // //populating members vector
    cout << "Type the name of each player in your team! " << endl << "You can only type one name at a time (then hit enter)" << endl;
    do
    {
        string player_name; 
        cin >> player_name;
        Player p(player_name);
        addPlayer(p); //using function thatt we have created instead of directly push_back
        //cout << num_player_ << endl;
    } while (num_player_ < 5);
    cout << endl;
    partyUpdate();
    cout << endl;
    //populating items in party class
    cout << "Now you have a party of 5! Stick together as your fate will be determined by this mysterious dungeon!" << endl << "You have 100 gold pieces to spend on these options" << endl;
    cout << endl;
    cout << "- INGREDIENTS. To make food, you have to cook raw ingredients." << endl;
    cout << "- COOKWARE. If you want to cook, you have to have cookware first." << endl;
    cout << "- WEAPONS. You'll want at least one weapon per party member to fend off monsters." << endl;
    cout << "- ARMOR. Armor increases the chances of surviving a monster attack." << endl;
    cout << endl;
    cout << "You can spend all of your money here before you start your journey, or you can save some to spend on merchants along the way. But beware, some of the merchants in this dungeon are shady characters, and they won't always give you a fair price..." << endl;
    cout << endl;
    cout << "If you're looking to get supplies, you've come to the right place." << endl << "I would be happy to part with some of my wares...for the proper price!" << endl << endl;
    cout << endl;
    inventoryUpdate();
    cout << endl;
    mainMerchant();
    //cout << "end start phase" << endl << endl;

    //populating vector of monster
    string mon_filename = "monsters.txt";
    readMonster(mon_filename);
    //cout << getNumMonster() << endl; 
    //populating vector of riddles
    string rid_filename = "riddles.txt";
    readRiddle(rid_filename);
    //cout << getNumRiddle() << endl;
    cout << endl;
    return;
}

//members
void Dungeon::addPlayer(Player player)
{
    members_.push_back(player);
    num_player_++;
}
Player Dungeon::getPlayerAt(int i)
{
    return members_.at(i);
}
int Dungeon::getNumPlayer()
{
    return num_player_;
}
void Dungeon::setNumPlayer()
{
    num_player_ = members_.size();
}
void Dungeon::setFullnessAt(int player_index, int new_fullness)
{
    vector<Player> &members = members_;
    members.at(player_index).setFullness(new_fullness);
    return;
}
int Dungeon::getFullnessAt(int player_index)
{
    return getPlayerAt(player_index).getFullness();
}
void Dungeon::removePlayer()
{
    for (int i=0; i< num_player_; i++)
    {
        if (getFullnessAt(i)<=0)
        {
            cout << "We're truly sorry. You've have lost " << getPlayerAt(i).getName() << endl;
            members_.erase(members_.begin()+i);
            num_player_--;
        }
    }
    return;
}
void Dungeon::removeWeapon() //need checking
{
    while (getNumWeapon()>getNumPlayer())
    {
        int rand_weapon = rand()%5;
        if (getPartyWeaponAt(rand_weapon)!=0)
        {
            setPartyWeaponAt(rand_weapon, getPartyWeaponAt(rand_weapon)-1);

            if(rand_weapon==0)
            {
                cout << "We are sorry. You lost a Stone Club!" << endl;
            }
            else if(rand_weapon==1)
            {
                cout << "We are sorry. You lost an Iron Spear!" << endl;
            }
            else if(rand_weapon==2)
            {
                cout << "We are sorry. You lost a (+1) Mythril Rapier!" << endl;
            }
            else if(rand_weapon==3)
            {
                cout << "We are sorry. You lost a (+2) Flaming Battle-Axe!" << endl;
            }
            else if(rand_weapon==4)
            {
                cout << "We are sorry. You lost a (+3) Vorpal Longsword!" << endl;
            }
        }
    }
    return;
}

//party
void Dungeon::setPartyCookwareAt(int index, int value)
{
    party_.setCookwareAt(index, value);
    return;
}
void Dungeon::setPartyCookwarePriceAt(int index, int value)
{
    party_.setCookwarePriceAt(index, value);
    return;
}
void Dungeon::setPartyWeaponAt(int index, int value)
{
    party_.setWeaponAt(index, value); 
    return;
}
void Dungeon::setPartyWeaponPriceAt(int index, int value)
{
    party_.setWeaponPriceAt(index, value); 
    return;
}
void Dungeon::setPartyTreasureAt(int index, int value)
{
    party_.setTreasureAt(index, value);
    return;
}
void Dungeon::setPartyTreasurePriceAt(int index, int value)
{
    party_.setTreasurePriceAt(index, value);
    return;
}
void Dungeon::setPartyGold(int value)
{
    party_.setGold(value);
}
void Dungeon::setPartyIngredient(int value)
{
    party_.setIngredient(value);
}
void Dungeon::setPartyIngredientPrice(int value)
{
    party_.setIngredientPrice(value);
}
void Dungeon::setPartyArmor(int value)
{
    party_.setArmor(value);
}
void Dungeon::setPartyArmorPrice(int value)
{
    party_.setArmorPrice(value);
}

int Dungeon::getPartyCookwareAt(int index)
{
    return party_.getCookwareAt(index);
}
int Dungeon::getPartyCookwarePriceAt(int index)
{
    return party_.getCookwarePriceAt(index);
}
int Dungeon::getPartyWeaponAt(int index)
{
    return party_.getWeaponAt(index);
}
int Dungeon::getPartyWeaponPriceAt(int index)
{
    return party_.getWeaponPriceAt(index);
}
int Dungeon::getPartyTreasureAt(int index)
{
    return party_.getTreasureAt(index);
}
int Dungeon::getPartyTreasurePriceAt(int index)
{
    return party_.getTreasurePriceAt(index);
}
int Dungeon::getPartyGold()
{
    return party_.getGold();
}
int Dungeon::getPartyIngredient()
{
    return party_.getIngredient();
}
int Dungeon::getPartyIngredientPrice()
{
    return party_.getIngredientPrice();
}
int Dungeon::getPartyArmor()
{
    return party_.getArmor();
}
int Dungeon::getPartyArmorPrice()
{
    return party_.getArmorPrice();
}
int Dungeon::getNumWeapon()
{
    int num_weapons_total=0;
    for (int i=0; i<5;i++) // to calculate the total number of weapons currently held by the party
    {
        num_weapons_total += getPartyWeaponAt(i);
    }
    return num_weapons_total;
}
int Dungeon::getNumCookware()
{
    int num_cookware=0;
    for (int i=0; i<3; i++)
    {
        num_cookware += getPartyCookwareAt(i);
    }
    return num_cookware;
}

//Monster
Monster Dungeon::getMonsterAt(int i)
{
    return mons_.at(i);
}
void Dungeon::readMonster(string filename) //reading file, slpit names and levels, and put them into the vector of monsters
{
    ifstream fin;
    fin.open(filename);
    string line;
    while (getline(fin, line))
    {
        Monster mon_read;
        mon_read.setMonster(line.substr(0,line.length()-2));
        mon_read.setLevel(int(line[line.length()-1])-48); //-48 becasue of ascii
        mons_.push_back(mon_read);
        num_mons_++;
    }
    fin.close();
    return;
}
void Dungeon::removeMonsterAt(int i)
{
    mons_.erase(mons_.begin()+i);
    num_mons_--;
    return;
}
int Dungeon::getNumMonster()
{
    return num_mons_;
}
void Dungeon::setNumMonster()
{
    num_mons_ = mons_.size();
    return;
}
int Dungeon::getNumDefeatedMonster()
{
    return num_mons_defeated_;
}
void Dungeon::setNumDefeatedMonster(int num_mons_defeated)
{
    num_mons_defeated_ = num_mons_defeated;
    return;
}
bool Dungeon::ishaveMonLevel(int mon_level)
{
    bool haveMon = false;
    for (int i=0; i<getNumMonster(); i++) //cheking whether the mon_ vector have monster of the corresponding level
    {
        if (getMonsterAt(i).getLevel() == mon_level)
        {
            haveMon = true;
            break;
        }
    }

    return haveMon;
}

//Riddles
void Dungeon::readRiddle(string filename)
{
    ifstream fin;
    fin.open(filename);
    string line;
    char separator = '~';
    while (getline(fin, line))
    {
        Riddle riddle_read;
        for (int i=0; i < line.length(); i++)
        {
            if (line[i] == separator)
            {
                riddle_read.setQuestion(line.substr(0,i));
                riddle_read.setAnswer(line.substr(i+1)); //i+1 becasue we dont want separator
                //riddle_vec.push_back(riddle_read);
                riddles_.push_back(riddle_read);
                break;
            } 
        }
        num_riddles_++;
    }
    fin.close();
    return;
}
Riddle Dungeon::getRiddleAt(int i)
{
    return riddles_.at(i);
}
int Dungeon::getNumRiddle()
{
    return num_riddles_;
}
void Dungeon::setNumRiddle()
{
    num_riddles_ = riddles_.size();
}
void Dungeon::removeRiddleAt(int i)
{
    riddles_.erase(riddles_.begin()+i);
}

//status
int Dungeon::getStatusAt(int index)
{
    return status_[index];
}
void Dungeon::setStatusAt(int index, int value)
{
    status_[index] = value;
}

//Map
Map Dungeon::getMap()
{
    return map_;
}

//giveup
bool Dungeon::getGiveup()
{
    return giveup_;
}
void Dungeon::setGiveup(bool giveup)
{
    giveup_ = giveup;
}
bool Dungeon::isInputInteger(string input)
{
    bool isValid = true;
    for (int i=0; i<input.length(); i++)
    {
        if (!isdigit(input[i]))
        {
            isValid = false;
            break;
        }
    }
    return isValid;
}
void Dungeon::inventoryUpdate() //done
{
    cout << "INVENTORY | " << endl;
    cout << "Gold | " << getPartyGold() << endl;
    cout << "Ingredient | " << getPartyIngredient() << endl;
    cout << "Cookware | " << "P: " << getPartyCookwareAt(0) << " | " << "F: " << getPartyCookwareAt(1) << " | " << "C: " << getPartyCookwareAt(2) << endl;
    cout << "Weapons | " << "C: " << getPartyWeaponAt(0) << " | " << "S: " << getPartyWeaponAt(1) << " | " << "R: " << getPartyWeaponAt(2) << " | " << "B: " << getPartyWeaponAt(3) << " | " << "L: " << getPartyWeaponAt(4) << endl;
    cout << "Armor | " << getPartyArmor() << endl;
    cout << "Treasure | " << "R: " << getPartyTreasureAt(0) << " | " << "N: " << getPartyTreasureAt(1) << " | " << "B: " << getPartyTreasureAt(2) << " | " << "C: " << getPartyTreasureAt(3) << " | " << "G: " << getPartyTreasureAt(4) << endl;
    return;
}
void Dungeon::partyUpdate() //done
{
    cout << "PARTY | " << endl;
    for (int i=0; i < getNumPlayer(); i++)
    {
        cout << getPlayerAt(i).getName() << " | " << "Fullness: " << getPlayerAt(i).getFullness() << endl; //getPlayerAt(i).getFullness() = getFullnessAt(i)
    }
    return;
}
bool Dungeon::probability(int prob_percent)//done
{
    int random = rand()%100 + 1; //giveing number between 1-100
    // 10% to occur -> yes (1-10), no (11-100)
    if (random <= prob_percent)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Dungeon::yesorno(string yn) //for buying from merchant
{
    bool check;
    while (!(yn == "y" || yn == "Y" || yn == "n" || yn == "N"))
    {
        cout << "Please enter y or n." << endl;
        cin >> yn;
    }
    if (yn == "y" || yn == "Y")
    {
        cout << "Thank you for your patronage! What else can I get for you?" << endl;
        check = true;
    }
    else if (yn == "n" || yn == "N")
    {
        cout << "I'm sorry you decided not to trade anything with us. What else can I get for you?" << endl;
        check = false;
    }
    return check;
}
void Dungeon::statusUpdate() //done
{
    cout << "STATUS | " << endl;
    cout << "Rooms Cleared: " << getStatusAt(0) << " | Keys: " << getStatusAt(1) << " | Anger Level: " << getStatusAt(2) << " | Number of steps: " << getStatusAt(3) << endl;
    cout << endl;
    inventoryUpdate();
    cout<< endl;
    partyUpdate();
    cout << endl;
    return;
}
bool Dungeon::isEndgame()// used to check after actions and move,(not sure about all the conditions)
{
    bool isEnd = false;
    if (getNumPlayer()==1)
    {
        isEnd = true; //lost
    }
    else if (getStatusAt(2)==100) //anger level at status[2]
    {
        isEnd = true; //lost
    }
    else if (getStatusAt(0)==5 && getNumPlayer()>1 && (map_.getPlayerCol()==map_.getDungeonExitCol() && map_.getPlayerRow()==map_.getDungeonExitRow())) // rooms cleared at status[0]
    {
        isEnd = true; //win
    }
    else if (getGiveup()==true)
    {
        isEnd = true;
    }
    return isEnd;
}
void Dungeon::moveAction() //done (dont forget to remove checking cout) ***inputValidation needs to be checked + it does not count step moving into the explored space
{
    bool loop = false;
    while (!loop)
    {
        cout << "Let's move to another space!!!" << endl << "Here is instruction for making a move!" << endl << "'w' to move up" << endl << "'s' to move down" << endl << "'a' to move left" << endl << "'d' to move rightt" << endl;
        char move_input; //give error easily using char -> using string is easier but have to implement move function
        cin >> move_input;
        while (!(move_input=='w' || move_input=='W' || move_input=='s' || move_input=='S' || move_input=='a' || move_input=='A' || move_input=='d' || move_input=='D') || cin.fail()) //***fail in case of inputting string "clear", it will go through the loop 3 time before reading 'a' as option for moving
        {
            cin.clear();
            cin.ignore(500,'\n'); //some how does not ignor
            cout << "You can use only 'w', 's', 'a', and 'd' for moving!" << endl;
            cin >> move_input;
        }
        bool move_return = map_.move(move_input);
        if (move_return==false)
        {
            cout << "You're moving out of the range!" << endl;
        }
        else
        {
            if (!(map_.getMapdata(map_.getPlayerRow(), map_.getPlayerCol())==' '))
            //if (!(map_.isExplored(map_.getPlayerRow(),map_.getPlayerCol()))) //condition for entering new NPC space should be +1, but right now it does not count
            {
                setStatusAt(3, getStatusAt(3)+1); //+1 number of turns when the new space is not already expored
            }

            //increment anger level (for every single move)
            setStatusAt(2, getStatusAt(2)+1);
            loop = true;
        }
        //these two are for testing
        // cout << "Step" << getStatusAt(3) << endl;
        // cout << "Anger level" << getStatusAt(2) << endl;
    }
    return;
}
void Dungeon::fightingMonster() //done, tested but not sure if it will always work (dont forget to remove checking couts)
{
    //validation check for the number of weapon and armor
    if (!(getNumWeapon()>0 && getPartyArmor() >0))
    {
        if (getNumWeapon()==0 && getPartyArmor()>0)
        {
            cout << "You don't have any weapon to fight a monster." << endl;
        }
        else if (getNumWeapon()>0 && getPartyArmor()==0)
        {
            cout << "You don't have any armor to protect your team fighting a monster." << endl;
        }
        else if (getNumWeapon()==0 && getPartyArmor()==0)
        {
            cout << "You don't have amy weapon and armor to fight a monster." << endl;
        }
        cout << "Please choose another option!" << endl;
        return;
    }

    int mon_level = getStatusAt(0)+1; //mon_level = 1 fightign before clearing room 1,.., thus clearing 5 rooms will get mon_level = 6 which is the sorceror
    if(!ishaveMonLevel(mon_level)) //does not have mon
    {
        cout << "You have defeated all monsters of level " << mon_level << " !!!" << endl;
        return; //terminate the function as soon as there is no monster of this level left.
    }

    int mon_index;
    bool loop = true;

    while (loop==true) //if there is no monster of that level -> haveMon == false and the while loop wont run -> loop=false
    {
        //finding the monster
        int rand_index = rand()%getNumMonster(); //keep randomizing the index until it give the monster at that index that has the level we want
        if (getMonsterAt(rand_index).getLevel()==mon_level) //making sure that monster's level at rand_index position has the level that we want
        {
            mon_index = rand_index; //assigning rand_index into mon_index which will be used later (for getting name, level, and removing)
            loop = false;
        }
    }
    cout << getMonsterAt(mon_index).getMonName() << " AHEAD!!! THEY LOOK HOSTILE! BE CAREFUL!" << endl; //dont need to be in the if condition becasue passing to this line already prove the conditino

    //fighting calculation
    int w = getNumWeapon() + getPartyWeaponAt(2) + 2*(getPartyWeaponAt(3)) + 3*(getPartyWeaponAt(4));
    int d=0;
    int r1 = rand()%6 + 1; //1-6
    int r2 = rand()%6 + 1;
    int c = mon_level;
    int a = getPartyArmor();

    //caldulating d is a problem (not sure if below code will correct -> logically)
    bool unique_weapon = true;
    for (int i=0; i<5; i++) //max num of weapon type = 5. Depending on the number of player 5 players have 5 different weapons, similary, 3 players must have 3 different weapons
    {//meaning that the number of weapon for each type has to be one for the party to have unique weapon
        if (getPartyWeaponAt(i) >1)
        {
            unique_weapon = false;
            break; //will this break out of the for loop
        }
    }
    if (unique_weapon==true && getNumPlayer()==getNumWeapon())
    {
        d=4;
    }
    //if not it's 0 by the definition (already initialized to 0 at the beginning)

    //outcome condition
    int cal_outcome = ((r1*w)+d)-((r2*c)/a);
    cout <<"w " << w << endl;
    cout <<"d " << d << endl;
    cout <<"r1 " << r1 << endl;
    cout <<"r2 " << r2 << endl;
    cout <<"c " << c << endl;
    cout <<"a " << a << endl;
    cout << "Testing " << cal_outcome << endl;
    if (cal_outcome>0)
    {
        removeMonsterAt(mon_index); //removing the monster only in case when u win it
        setNumMonster(); //updating just in case (it will set it by getting the length of vector of monsters)
        //cout << "Number of monsters: "<< getNumMonster() << endl;
        //cout << "Number of monsters defeated: " << getNumDefeatedMonster() << endl;
        setNumDefeatedMonster(getNumDefeatedMonster()+1);
        cout << "You did it!!! You have defeated the monster!!!" << endl;
        //party_.setGold(party_.getGold()+(10*c));
        setPartyGold(getPartyGold() + (10*c));
        cout << "You recieve " << 10*c << " golds as a prize!" << " (" << getPartyGold() << " in total)" << endl;
        //party_.setIngredient(party_.getIngredient())
        setPartyIngredient(getPartyIngredient() + (5*c));
        cout << "You recieve " << 5*c << " ingredients as a prize!" << " (" << getPartyIngredient() << " in total)" << endl;
        bool isKey = probability(10);
        if (isKey==true)
        {
            cout << "Wait! The monster dropped the key! Lucky you!" << endl;
            setStatusAt(1, getStatusAt(1)+1); //status[1] = key
            cout << "You currently have " << getStatusAt(1) << " key(s)" << endl;
        }
    }
    else
    {
        //not removing monster
        //NOTE not adding number of defeated monstes
        cout << "The monster is too strong! You've lost the fight." << endl;
        //losing gold
        int gold_remained = getPartyGold()*0.75;
        setPartyGold(gold_remained);
        cout << "You lost some gold " << getPartyGold() << " golds are remained." << endl;
        //losing ingredient
        if (getPartyIngredient()<=30) //losing ingredient up to 30. This means that having less than 30 will have 0 remained
        {
            cout << "You have lost all the ingredient." << endl;
            setPartyIngredient(0);
        }
        else
        {
            cout << "You have lost 30 ingredients" << endl;
            setPartyIngredient(getPartyIngredient()-30); //having more than 30 will be substracted by 30
        }
        //losing member
        for (int i=1; i<getNumPlayer(); i++) //i=1 for excluding the party leader
        {
            if(getPartyArmor() > 0) //5% chance
            {
                bool isSlain = probability(5);
                if(isSlain==true)
                {
                    cout << getPlayerAt(i).getName() << " was killed by the monster!" << endl;
                    members_.erase(members_.begin()+i);
                    setNumPlayer(); //just in case that of using getNumPlayer later
                }
            }
            else //10% chance
            {
                bool isSlain = probability(10);
                if(isSlain==true)
                {
                    cout << getPlayerAt(i).getName() << " was killed by the monster!" << endl;
                    members_.erase(members_.begin()+i);
                    setNumPlayer(); //just in case that of using getNumPlayer later
                }
            }
        }
        //removePlayer(); //this function will print out people you've lost (people who die from being slained rather than by hunger)
        removeWeapon(); //akways checks after losing player
    }
    return;
}
void Dungeon::giveUp() //done
{
    cout << "I understand that escaping this dungeon is not easy, but do you really want to give up? (y/n)" << endl;
    string yn;
    cin >> yn;
    while (!(yn == "y" || yn == "Y" || yn == "n" || yn == "N"))
    {
        cout << "Please enter 'y' to confirm that you want to give up, otherwise enter 'n'." << endl;
        cin >> yn;
    }
    if(yn == "y" || yn == "Y")
    {
        cout << "We're so sorry that you have given up." << endl;
        setGiveup(true);
    }
    else if (yn == "n" || yn == "N")
    {
        cout << "I knew that you wouldn't give up so easily!!!" << endl;
    }
    return;
}
bool Dungeon::hungerWarning() //this one has to be at the beginning of each turns -> using bool becasue it will connect to whether you gonna erase player (dying by hunger) at the end or not
{
    bool isWarning = false;
    for (int i=0; i<getNumPlayer(); i++)
    {
        if (getFullnessAt(i)==0)
        {
            cout << "Warning: " << getPlayerAt(i).getName() << " is on the blink of starvation!" << endl << "You should cook and eat some food!" << endl;
            isWarning = true;
        }
    }
    return isWarning;
    //***death by hunger printing (maybe another function)
    //OH NO! Morgan has died of hunger!
    //Your party size has reduced to 4 members.
}
int Dungeon::getNumExplored() //done but not tested
{

    int num_explored = 0;
    for (int i=0; i<12; i++)//row
    {
        for (int j=0; j<12; j++)//col
        {
            if (map_.getMapdata(i,j) == ' ')
            {
                num_explored++;
            }
        }
    }
    return num_explored;
}

void Dungeon::mainNormalSpace() //not done,
{
    bool loop = true;
    while(loop)
    {
        printNormalSpaceActions();
        int main_choice;
        string main;
        cin >> main;
        cout << endl;
        // while (!(main_choice>=1 && main_choice<=5) || cin.fail())
        // {
        //     cin.clear(); 
        //     cin.ignore(100,'\n'); //these two lines will prevent while from running infinite becasue of not inputing integer. 512 mean will clear 512 character of the input
        //     printNormalSpaceActions();
        //     cout << "Please enter number between 1 and 5" << endl;
        //     cin >> main_choice;
        // }

        bool isInputValid = false;
        while (!isInputValid) //this while loop is fucking important!!!!!!!
        {
            while (isInputInteger(main)==false)
            {
                cout << "Please enter integer." << endl;
                cin >> main;
            }

            if (stoi(main)<1 || stoi(main)>5)
            {
                cout << "Please enter number between 1 and 5" << endl;
                cin >> main;
            }
            else
            {
                isInputValid = true; //breaking the loop when the input is valid
            }
        }

        main_choice = stoi(main);
        switch (main_choice)
        {
            case 1: //done but not checked
                //each player has a 20% chance of dropping fullness by 1
                for (int i=0; i < getNumPlayer(); i++) //looping through each player in vector
                {
                    bool isDrop = probability(20); //calculating new bool for each loop -> each player has independent chance
                    if (isDrop == true)
                    {
                        if (getFullnessAt(i)>0) //becasue fullness cannot be less than 0
                        {
                            setFullnessAt(i, getFullnessAt(i)-1);
                            cout << getPlayerAt(i).getName() << "'s fullness has dropped by 1." << endl;
                        }
                    }
                }
                loop = false; // to break the loop and the move action will occur at the new while loop in mainGame
                break;
            case 2:
                normalSpaceInvestigate();
                break;
            case 3: 
                //condiditon -> the number of monster (specific level corresponding to the room clears) cannot be 0; -> not sure if fightingMonster funnction already includes this condition
                normalSpacePickfight();
                break;
            case 4:
                normalSpaceCookeat(); //done
                break;
            case 5: //passed
                giveUp(); 
                if(getGiveup()==true) //going out as soon as giving up
                {
                    cout << endl;
                    return;
                }
                break;
        }
    }
    cout << endl;
    return;
}
void Dungeon::printNormalSpaceActions() //done
{
    cout << "Here are things you can do! Choose wisely and the luck will be yours!" << endl;
    cout << "1. Move" << endl << "2. Investigate" << endl << "3. Pick a fight" << endl << "4. Cook and Eat" << endl << "5. Give up" << endl;
    return;
}
void Dungeon::normalSpaceInvestigate() // using members_erase for removeing immediatetly instead of remove function becasue this will prevent remove() from removing player whose fullness at 0 because of hunger
{
    if(!(map_.isExplored(map_.getPlayerRow(), map_.getPlayerCol()))) //if it's not explored
    {//dont forget to set this space to be explored at the end
        bool isKeyFound = probability(10);
        if (isKeyFound==true)
        {
            cout << "You have found a key for secret rooms!!! Keep it tight in your hand and try not to get it stolen!!!" << endl;
            setStatusAt(1, getStatusAt(1)+1); //index 1 of Status is the number of key
        }
        else
        {
            cout << "Sorry, you did not find a key from exploring. Let's try investigating other spaces!!!" << endl; 
        }

        bool isTreasureFound = probability(20);
        if (isTreasureFound==true)
        {
            switch(getStatusAt(0)) //number of rooms cleared
            {
                case 0:
                    cout << "You're pretty close to find a treasure!!! Let's explore more secret rooms to find rare tresure!" << endl;
                    break;
                case 1:
                    cout << "Look what you've found!!! It's a Silver Ring!!!" << endl;
                    setPartyTreasureAt(0, getPartyTreasureAt(0)+1);
                    break;
                case 2:
                    cout << "Look what you've found!!! It's a Ruby necklace!!!" << endl;
                    setPartyTreasureAt(1, getPartyTreasureAt(1)+1);
                    break;
                case 3:
                    cout << "Look what you've found!!! It's a Emerald bracelet!!!" << endl;
                    setPartyTreasureAt(2, getPartyTreasureAt(2)+1);
                    break;
                case 4:
                    cout << "Look what you've found!!! It's a Diamond circlet!!!" << endl;
                    setPartyTreasureAt(3, getPartyTreasureAt(3)+1);
                    break;
                case 5:
                    cout << "Look what you've found!!! It's a Gem-encrusted goblet!!!" << endl;
                    setPartyTreasureAt(4, getPartyTreasureAt(4)+1);
                    break;
            }
        }
        else
        {
            cout << "Sorry, you did not find any treasure from exploring. Let's try investigating other spaces!!!" << endl;
        }
        //number of weapon has to be at least 1
        bool isMonster = probability(20);
        if (isMonster==true && ishaveMonLevel(getStatusAt(0)+1)==true && getNumWeapon()>0 && getPartyArmor()>0)
        {
            //******** You have defeated all the monster in this level
            cout << "You have found a hostile monster!!! You choose either to fight or surrender!!!" << endl;
            cout << "1. To Fight!!!" << endl << "2. To Surrender!!!" << endl;
            string fight_surr;
            cin >> fight_surr;
            while(!(fight_surr == "1" || fight_surr=="2"))
            {
                cout << "You can either choose 1. To Fight or 2. To Surrender" << endl;
                cin >> fight_surr;
            }
            if (fight_surr == "1")
            {
                fightingMonster();
            }
            else if (fight_surr=="2")
            {
                //surrender function
                //to lose one member that is not leader -> using rand() for finding the index that is not zero, setting their fullness to zero and use removePlayer
                cout << "Monster is too strong! It's ok to surrender. You party will win next time!!!" << endl;
                int index = rand()%(getNumPlayer()-1) + 1; //gonna give the range that exclude 0. For example, if num_player=5 -> rand()%4+1 will be 1-4.
                cout  << getPlayerAt(index).getName() << " has been held captive by the monster! You must continue without them!" << endl;
                members_.erase(members_.begin()+index);
                setNumPlayer(); //just in case that of using getNumPlayer later
                //removePlayer(); //remove here right away becasue it's not death by hunger but slained by the monster
                removeWeapon();
            }
            //after facing monster -> 50% chance that all players' fullness decrease by 1
            bool isFullnessDecrease = probability(50);
            if (isFullnessDecrease==true)
            {
                cout << "You party's fullness has dropped by 1. (after fighting a monster)" << endl;
                for (int i=0; i< getNumPlayer(); i++) //this does not mean that it's 0 becasue monster kill them -> so it's considered death by hunger
                {//****this loop give out of range
                    if (getFullnessAt(i)>0)
                    {
                        setFullnessAt(i, getFullnessAt(i)-1);
                    }
                }
                cout << "Error check for surrender's fullness decrease for loop" << endl;
            }
        }
        else //will go through this one if the party does not fight with monster
        {
            cout << "Good luck for you! There is no monster in this space!" << endl;
            bool isFullnessDecrease = probability(50);
            if (isFullnessDecrease==true)
            {
                cout << "You party's fullness has dropped by 1. (after investigating a space.)" << endl;
                for (int i=0; i< getNumPlayer(); i++)
                {
                    if (getFullnessAt(i)>0)
                    {
                        setFullnessAt(i, getFullnessAt(i)-1);
                    }
                }
            }
        }
        map_.exploreSpace(map_.getPlayerRow(), map_.getPlayerCol()); //marking space to be explored
    }
    else
    {
        cout << "This space has been explored! Please choose different option!" << endl;
    }
    //removePlayer(); //removing player(s) and print out the names after the fight/investigating -> dont need this becasue dying by monster already have removePlayer()
    cout << endl;
    return;
}
void Dungeon::normalSpaceCookeat() //done tested
{
    //ingredient != 0 && numCookware != 0
    if (getPartyIngredient()<=0&&getNumCookware()<=0)
    {
        cout << "We're so sorry, you don't have any ingredient and cookware for making food!" << endl;
        cout << endl;
        return;
    }
    else if (getPartyIngredient()>0&&getNumCookware()<=0)
    {
        cout << "You have some ingredient! Sadly, you don't have any cookware to use." << endl;
        cout << endl;
        return;
    }
    else if (getPartyIngredient()<=0&&getNumCookware()>0)
    {
        cout << "You have some cookwares! Sadly, you don't have any ingredient to use." << endl;
        cout << endl;
        return;
    }
    //only haveing both will not counter the previous return(s) = passed

    //prompting user to choose amount of ingredient
    cout << "You have " << getPartyIngredient() << " kg of ingredient. The ratio of successful cooking is 1 fullness: 5 kg." << endl << "How much do you want to cook?" << endl;
    int amount_ing;
    string ing;
    cin >> ing; //***probably gonna give an error when inputing wrong type of data type -> infinite loop for not int data type
    // while (!(amount_ing%5==0 && amount_ing <= getPartyIngredient()))
    // {
    //     cout << "Please enter an integer that is multiple of 5, and make sure that you have enough ingredient to cook." << endl;
    //     cin >> amount_ing;
    // }
    bool isInputValid = false;
        while (!isInputValid) //this while loop is fucking important!!!!!!!
        {
            while (isInputInteger(ing)==false)
            {
                cout << "Please enter integer." << endl;
                cin >> ing;
            }

            if (!(stoi(ing)%5==0 && stoi(ing) <= getPartyIngredient()))
            {
                cout << "Please enter an integer that is multiple of 5, and make sure that you have enough ingredient to cook." << endl;
                cin >> ing;
            }
            else
            {
                isInputValid = true; //breaking the loop when the input is valid
            }
        }
    amount_ing = stoi(ing);
    if(amount_ing==0)
    {
        cout << "We are sorry you don't want to cook any food." << endl;
        cout << endl;
        return;
    }

    //promting user to choose cookware
    bool not_pass = true;
    string cw; //to be input
    while (not_pass)
    {
        //cout << "You have " << getPartyIngredient() << " kg of ingredient." << endl;
        cout << "Here is the list of your current cookware types and available amount!" << endl;
        cout << "1. Ceramic pot: " << getPartyCookwareAt(0) << " (25% chance of breaking)" << endl;
        cout << "2. Frying pan: " << getPartyCookwareAt(1) << " (10% chance of breaking)" << endl;
        cout << "3. Cauldron: " << getPartyCookwareAt(2) << " (2% chance of breaking)" << endl;
        cout << "Please enter 1, 2, or 3 for choosing coressponding type of cookware to be used." << endl;
        cin >> cw;
        while (!(cw=="1" || cw=="2" || cw=="3"))
        {
            cout << "Please enter 1, 2, or 3." << endl;
            cin >> cw;
        }

        if (stoi(cw)==1 && getPartyCookwareAt(0)<=0)
        {
            cout << "You don't have a ceramic pot to use. Please choose another cookware." << endl;
        }
        else if (stoi(cw)==2 && getPartyCookwareAt(1)<=0)
        {
            cout << "You don't have a frying pan to use. Please choose another cookware." << endl; 
        }
        else if (stoi(cw)==3 && getPartyCookwareAt(2)<=0)
        {
            cout << "You don't have a cauldron to use. Please choose another cookware." << endl; 
        }
        else
        {
            not_pass = false; // passed
        }
    }

    int cw_index = stoi(cw)-1;
    if (cw=="1")
    {
        bool isBreak = probability(25);
        if (isBreak==true)
        {
            cout << "NOOOOO! a ceramic pot has broken! You have lost it and " << amount_ing << " kg of ingredient." << endl;
            setPartyCookwareAt(cw_index, getPartyCookwareAt(cw_index)-1);//losing cookware
            setPartyIngredient(getPartyIngredient()-amount_ing);//losing ingredient
        }
        else
        {
            cout << "You have successfully cook " << amount_ing << " kg of ingredient!" << endl << "everyone's fullness has increased by " << amount_ing/5 << endl;
            for (int i=0; i< getNumPlayer(); i++)
            {
                setFullnessAt(i, getFullnessAt(i)+(amount_ing/5));
            }
            setPartyIngredient(getPartyIngredient()-amount_ing);
            // partyUpdate();
            // cout << "You have " << getPartyIngredient() << " kg of ingredient left." << endl;
        }
    }
    else if (cw=="2")
    {
        bool isBreak = probability(10);
        if (isBreak==true)
        {
            cout << "NOOOOO! a frying has broken! You have lost it and " << amount_ing << " kg of ingredient." << endl;
            setPartyCookwareAt(cw_index, getPartyCookwareAt(cw_index)-1);//losing cookware
            setPartyIngredient(getPartyIngredient()-amount_ing);//losing ingredient
        }
        else
        {
            cout << "You have successfully cook " << amount_ing << " kg of ingredient!" << endl << "everyone's fullness has increased by " << amount_ing/5 << endl;
            for (int i=0; i< getNumPlayer(); i++)
            {
                setFullnessAt(i, getFullnessAt(i)+(amount_ing/5));
            }
            setPartyIngredient(getPartyIngredient()-amount_ing);
            // partyUpdate();
            // cout << "You have " << getPartyIngredient() << " kg of ingredient left." << endl;
        }
    }
    else if (cw=="3")
    {
        bool isBreak = probability(2);
        if (isBreak==true)
        {
            cout << "NOOOOO! a frying has broken! You have lost it and " << amount_ing << " kg of ingredient." << endl;
            setPartyCookwareAt(cw_index, getPartyCookwareAt(cw_index)-1);//losing cookware
            setPartyIngredient(getPartyIngredient()-amount_ing);//losing ingredient
        }
        else
        {
            cout << "You have successfully cook " << amount_ing << " kg of ingredient!" << endl << "everyone's fullness has increased by " << amount_ing/5 << endl;
            for (int i=0; i< getNumPlayer(); i++)
            {
                setFullnessAt(i, getFullnessAt(i)+(amount_ing/5));
            }
            setPartyIngredient(getPartyIngredient()-amount_ing);
            // partyUpdate();
            // cout << "You have " << getPartyIngredient() << " kg of ingredient left." << endl;
        }
    }
    partyUpdate();
    cout << "You have " << getPartyIngredient() << " kg of ingredient left." << endl;
    cout << endl;
    return;
}
void Dungeon::normalSpacePickfight() //probably the same as fightingMonster -> no need another function then
{
    //validation check for the number of weapon and armor
    if (!(getNumWeapon()>0 && getPartyArmor() >0))
    {
        if (getNumWeapon()==0 && getPartyArmor()>0)
        {
            cout << "You don't have any weapon to fight a monster." << endl;
        }
        else if (getNumWeapon()>0 && getPartyArmor()==0)
        {
            cout << "You don't have any armor to protect your team fighting a monster." << endl;
        }
        else if (getNumWeapon()==0 && getPartyArmor()==0)
        {
            cout << "You don't have amy weapon and armor to fight a monster." << endl;
        }
        cout << "Please choose another option!" << endl;
        cout << endl;
        return;
    }

    int mon_level = getStatusAt(0)+1; //mon_level = 1 fightign before clearing room 1,.., thus clearing 5 rooms will get mon_level = 6 which is the sorceror
    if(!ishaveMonLevel(mon_level)) //does not have mon
    {
        cout << "You have defeated all monsters of level " << mon_level << " !!!" << endl;
        cout << endl;
        return; //terminate the function as soon as there is no monster of this level left.
    }

    int mon_index;
    bool loop = true;

    while (loop==true) //if there is no monster of that level -> haveMon == false and the while loop wont run -> loop=false
    {
        //finding the monster
        int rand_index = rand()%getNumMonster(); //keep randomizing the index until it give the monster at that index that has the level we want
        if (getMonsterAt(rand_index).getLevel()==mon_level) //making sure that monster's level at rand_index position has the level that we want
        {
            mon_index = rand_index; //assigning rand_index into mon_index which will be used later (for getting name, level, and removing)
            loop = false;
        }
    }
    cout << getMonsterAt(mon_index).getMonName() << " AHEAD!!! THEY LOOK HOSTILE! BE CAREFUL!" << endl; //dont need to be in the if condition becasue passing to this line already prove the conditino

    //fighting calculation
    int w = getNumWeapon() + getPartyWeaponAt(2) + 2*(getPartyWeaponAt(3)) + 3*(getPartyWeaponAt(4));
    int d=0;
    int r1 = rand()%6 + 1; //1-6
    int r2 = rand()%6 + 1;
    int c = mon_level;
    int a = getPartyArmor();

    //caldulating d is a problem (not sure if below code will correct -> logically)
    bool unique_weapon = true;
    for (int i=0; i<5; i++) //max num of weapon type = 5. Depending on the number of player 5 players have 5 different weapons, similary, 3 players must have 3 different weapons
    {//meaning that the number of weapon for each type has to be one for the party to have unique weapon
        if (getPartyWeaponAt(i) >1)
        {
            unique_weapon = false;
            break; //will this break out of the for loop
        }
    }
    if (unique_weapon==true)
    {
        d=4;
    }
    //if not it's 0 by the definition (already initialized to 0 at the beginning)

    //outcome condition
    int cal_outcome = ((r1*w)+d)-((r2*c)/a);
    cout <<"w " << w << endl;
    cout <<"d " << d << endl;
    cout <<"r1 " << r1 << endl;
    cout <<"r2 " << r2 << endl;
    cout <<"c " << c << endl;
    cout <<"a " << a << endl;
    cout << "Testing " << cal_outcome << endl;
    if (cal_outcome>0)
    {
        removeMonsterAt(mon_index); //removing the monster only in case when u win it
        setNumMonster(); //updating just in case (it will set it by getting the length of vector of monsters)
        //cout << "Number of monsters: "<< getNumMonster() << endl;
        //cout << "Number of monsters defeated: " << getNumDefeatedMonster() << endl;
        setNumDefeatedMonster(getNumDefeatedMonster()+1);
        cout << "You did it!!! You have defeated the monster!!!" << endl;
        //party_.setGold(party_.getGold()+(10*c));
        setPartyGold(getPartyGold() + (10*c));
        cout << "You recieve " << 10*c << " golds as a prize!" << " (" << getPartyGold() << " in total)" << endl;
        //party_.setIngredient(party_.getIngredient())
        setPartyIngredient(getPartyIngredient() + (5*c));
        cout << "You recieve " << 5*c << " ingredients as a prize!" << " (" << getPartyIngredient() << " in total)" << endl;
        bool isKey = probability(10);
        if (isKey==true)
        {
            cout << "Wait! The monster dropped the key! Lucky you!" << endl;
            setStatusAt(1, getStatusAt(1)+1); //status[1] = key
            cout << "You currently have " << getStatusAt(1) << " key(s)" << endl;
        }
    }
    else
    {
        //not removing monster
        //NOTE not adding number of defeated monstes
        cout << "The monster is too strong! You've lost the fight." << endl;
        //losing gold
        int gold_remained = getPartyGold()*0.75;
        setPartyGold(gold_remained);
        cout << "You lost some gold " << getPartyGold() << " golds are remained." << endl;
        //losing ingredient
        if (getPartyIngredient()<=30) //losing ingredient up to 30. This means that having less than 30 will have 0 remained
        {
            cout << "You have lost all the ingredient." << endl;
            setPartyIngredient(0);
        }
        else
        {
            cout << "You have lost 30 ingredients" << endl;
            setPartyIngredient(getPartyIngredient()-30); //having more than 30 will be substracted by 30
        }
        //losing member
        for (int i=1; i<getNumPlayer(); i++) //i=1 for excluding the party leader
        {
            if(getPartyArmor() > 0) //5% chance
            {
                bool isSlain = probability(5);
                if(isSlain==true)
                {
                    cout << getPlayerAt(i).getName() << " was killed by the monster!" << endl;
                    members_.erase(members_.begin()+i);
                    setNumPlayer(); //just in case that of using getNumPlayer later
                }
            }
            else //10% chance
            {
                bool isSlain = probability(10);
                if(isSlain==true)
                {
                    cout << getPlayerAt(i).getName() << " was killed by the monster!" << endl;
                    members_.erase(members_.begin()+i);
                    setNumPlayer(); //just in case that of using getNumPlayer later
                }
            }
        }
        if(probability(50))//50% chance everyone fullness drops by 1 
        {
            cout << "Your party's fullness has dropped by one." << endl;
            partyUpdate();
            cout << getNumPlayer() << endl;
            for (int i=0; i<getNumPlayer(); i++) //**out of range error
            {
                if (getFullnessAt(i)>0)
                {
                    setFullnessAt(i, getFullnessAt(i)-1);
                }
            }
            cout << endl;
            partyUpdate();
        }
        //removePlayer(); //this function will print out people you've lost (people who die from being slained rather than by hunger)
        removeWeapon(); //akways checks after losing player
    }
    cout << endl;
    return;
}

void Dungeon::mainMerchant()
{
    //maybe putting some welcoming statement before starting the game
    bool loop = true;
    while (loop)
    {
        printMerchant();
        int main_choice;
        string main;
        cin >> main;

        bool isInputValid = false;
        while (!isInputValid) //this while loop is fucking important!!!!!!!
        {
            while (isInputInteger(main)==false)
            {
                cout << "Please enter integer." << endl;
                cin >> main;
            }

            if (!(stoi(main) >= 1 && stoi(main) <= 6))
            {
                cout << "Please enter a positive mulitple of 5, or 0 to cancel" << endl;
                cin >> main;
            }
            else
            {
                isInputValid = true; //breaking the loop when the input is valid
            }
        }
        cout << endl;

        main_choice=stoi(main);
        switch (main_choice)
        {
            case 1:
                merchant_ingredient();
                break;
            case 2:
                merchant_cookware();
                break;
            case 3:
                merchant_weapon();
                break;
            case 4:
                merchant_armor();
                break;
            case 5:
                merchant_sell();
                break;
            case 6:
                cout << "Are you sure you're finished? You won't be able to buy anything else from me! (y/n)" << endl;
                string yn;
                cin >> yn;
                if(yesorno(yn))
                {
                    cout << "Stay safe out there! I hope you find a way to escape this dungeon! Goodbye!" << endl;
                    loop = false;
                }
                break;
        }
    }
    return;
}
void Dungeon::printMerchant()
{
    cout << "Let's make some modifications too your party!!! Here are things you can do! Be prepared for a long journey!!!" << endl;
    cout << "1. Ingredients: To make food, you have to cook raw ingredients." << endl;
    cout << "2. Cookware: You will need something to cook those ingredients." << endl;
    cout << "3. Weapons: It's dangerous to go alone, take this!" << endl;
    cout << "4. Armor: If you want to survive monster attacks, you will need some armor." << endl;
    cout << "5. Sell treasures: If you find anything shiny, I would be happy to take it off your hands." << endl;
    cout << "6. Leave: Make sure you get everything you need, I'm leaving after this sale!" << endl;
    return;
}
void Dungeon::merchant_ingredient()
{
    //setStatusAt(0,4); //setting rooms cleared to be 2 for testing
    int ingredient_price = (getPartyIngredientPrice()*(1+(getStatusAt(0)*0.25))); //getStatusAt(0) gives the number of room cleared
    cout << "I see that you need ingredients! How many kg of ingredients do you need? It's " << ingredient_price << " Gold/kg." << endl;
    cout << "(Enter a positive mulitple of 5, or 0 to cancel)" << endl;
    string ingre;
    cin >> ingre;
    cout << endl;
    //while making sure that the input is the integer
    //while mnaking sure that the integer is positive multiple of 5
    bool isInputValid = false;
    while (!isInputValid) //this while loop is fucking important!!!!!!!
    {
        while (isInputInteger(ingre)==false)
        {
            cout << "Please enter integer." << endl;
            cin >> ingre;
        }

        if (!(stoi(ingre)%5 == 0 && stoi(ingre) >= 0))
        {
            cout << "Please enter a positive mulitple of 5, or 0 to cancel" << endl;
            cin >> ingre;
        }
        else
        {
            isInputValid = true; //breaking the loop when the input is valid
        }
    } //pssing this while loop will make sure that the input is string of integer that is 0 or multiple of 5

    int ingre_amount;
    ingre_amount = stoi(ingre); //changing the string into the int
    if (ingre_amount==0)
    {
        cout << "We are sad that you decided not to buy any ingredient from us. What else can I get for you?" << endl;
        cout << endl;
        return; //this will be out of the function and start again att the main merchant menu
    }
    else
    {
        int cost = ingre_amount*ingredient_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry!, you don't have enough gold for buying " << ingre_amount << " kg of ingredients" << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return; //this will be out of the function and start again att the main merchant menu
        }

        cout << "You want to buy " << ingre_amount << " kg of ingredients for " << cost << " gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold((getPartyGold())-cost);
            setPartyIngredient(ingre_amount);
            cout << endl;
            inventoryUpdate();
        }
    }
    cout << endl;
    return;
}
void Dungeon::merchant_cookware()
{
    //setStatusAt(0, 4); // for testing
    // for (int i=0; i<3; i++) //setting 
    // {
    //     setPartyCookwarePriceAt(i, getPartyCookwarePriceAt(i)*(1+(getStatusAt(0)*0.25))); 
    // }
    int pot_price = (getPartyCookwarePriceAt(0)*(1+(getStatusAt(0)*0.25))); //we dont want based price (data member) to change
    int pan_price = (getPartyCookwarePriceAt(1)*(1+(getStatusAt(0)*0.25)));
    int cauldron_price = (getPartyCookwarePriceAt(2)*(1+(getStatusAt(0)*0.25)));

    cout << "I have a several types of cookware, which one would you like?" << endl << "Each type has a different probability of breaking when used, marked with (XX%)." << endl;
    cout << "Choose on of the following cokwares:" << endl;
    cout << "1. (25%) Ceramic Pot [" << pot_price << " Gold]" << endl;
    cout << "2. (10%) Frying Pan [" << pan_price << " Gold]" << endl;
    cout << "3. ( 2%) Cauldron [" << cauldron_price << " Gold]" << endl;
    cout << "4. Cancel" << endl;
    string cw_type;
    cin >> cw_type;
    while (!(cw_type == "1" || cw_type == "2" || cw_type == "3" || cw_type == "4")) //connsider the length becasue it gives error for white space -> does nto work
    {
        cout << "Please enter number between 1-4" << endl;
        cin >> cw_type;
    }

    if (cw_type=="4")
    {
        cout << "We are sad that you decided not to buy any cookware from us. What else can I get for you?" << endl;
        cout << endl;
        inventoryUpdate();
        cout << endl;
        return;
    }

    cout << "How many would you like? (Enter a positive integer, or 0 to cancel)" << endl; //this whole part is places outside of if causes becasue I dont want to write several times in those conditions
    string cook;
    cin >> cook;

    bool isInputValid = false;
    while (!isInputValid) //this while loop is fucking important!!!!!!!
    {
        while (isInputInteger(cook)==false)
        {
            cout << "Please enter integer. or 0 to cencel" << endl;
            cin >> cook;
        }

        if (!(stoi(cook) >= 0))
        {
            cout << "Please enter a positive mulitple of 5, or 0 to cancel" << endl;
            cin >> cook;
        }
        else
        {
            isInputValid = true; //breaking the loop when the input is valid
        }
    }

    int cook_amount;
    cook_amount=stoi(cook);
    if(cook_amount==0)
    {
        cout << "We are sad that you decided not to buy any cookware from us. What else can I get for you?" << endl;
        cout << endl;
        return;
    }
    
    if (cw_type=="1")
    {
        int cost = cook_amount*pot_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry!, you don't have enough gold for buying " << cook_amount << " Ceremic Pot(s)." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return; //this will be out of the function and start again att the main merchant menu
        }
        cout << "You want to buy " << cook_amount << " Ceramic Pot(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() - cost);
            setPartyCookwareAt(0, (getPartyCookwareAt(0))+cook_amount);
            cout << endl;
            inventoryUpdate();
        }
        cout << endl;
        return;
    }
    else if (cw_type=="2")
    {
        int cost = cook_amount*pan_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry!, you don't have enough gold for buying " << cook_amount << " Frying Pan(s)." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return; //this will be out of the function and start again att the main merchant menu
        }
        cout << "You want to buy " << cook_amount << " Frying Pan(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() - cost);
            setPartyCookwareAt(1, (getPartyCookwareAt(1))+cook_amount);
            cout << endl;
            inventoryUpdate();
        }
        cout << endl;
        return;
    }
    else if (cw_type=="3")
    {
        int cost = cook_amount*cauldron_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry!, you don't have enough gold for buying " << cook_amount << " Cauldron(s)." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return; //this will be out of the function and start again att the main merchant menu
        }
        cout << "You want to buy " << cook_amount << " Cauldron(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() - cost);
            setPartyCookwareAt(2, (getPartyCookwareAt(2))+cook_amount);
            cout << endl;
            inventoryUpdate();
        }
        cout << endl;
        return;
    }
    cout << endl;
    return;
}
void Dungeon::merchant_weapon() //have to work on the condition of removing weapon to upgrade the weapon -> is it requireed by the game?
{
    if (getNumWeapon()>=getNumPlayer())
    {
        cout << "Your party is holding " << getNumWeapon() << " weapons! You cannot buy more of them!" << endl;
        cout << endl;
        return;
    }

    int club_price = getPartyWeaponPriceAt(0)*(1+(getStatusAt(0)*0.25));
    int spear_price = getPartyWeaponPriceAt(1)*(1+(getStatusAt(0)*0.25));
    int rapier_price = getPartyWeaponPriceAt(2)*(1+(getStatusAt(0)*0.25));
    int axe_price = getPartyWeaponPriceAt(3)*(1+(getStatusAt(0)*0.25));
    int sword_price = getPartyWeaponPriceAt(4)*(1+(getStatusAt(0)*0.25));
    
    //I was thinking about removing lower level weapons to buy new +1/+2/+3 weapons -> thils whole condition will be hella messy
    cout << "I have a plentiful collection of weapons to choose from, what would you like?" << endl << "Note that some of them provide you a special bonus in combat, marked by a (+X)." << endl;
    cout << "Each player can hold only one weapon! You party can have up too 5 weapons!" << endl;
    cout << "Having unique weapon for each player in your party will increase the chance of winning monsters!!!" << endl; 
    cout << "Choose on of the following weapons:" << endl;
    cout << "1. Stone Club [" << club_price << " Gold]" << endl;
    cout << "2. Iron Spear [" << spear_price << " Gold]" << endl;
    cout << "3. (+1) Mythril Rapier [" << rapier_price << " Gold]" << endl;
    cout << "4. (+2) Flaming Battle-Axe [" << axe_price << " Gold]" << endl;
    cout << "5. (+3) Vorpal Longsword [" << sword_price << " Gold]" << endl;
    cout << "6. Cancel" << endl;
    string weapon_type;
    cin >> weapon_type;
    while (!(weapon_type == "1" || weapon_type == "2" || weapon_type == "3" || weapon_type == "4" || weapon_type == "5" || weapon_type == "6"))
    {
        cout << "Please enter number between 1-6" << endl;
        cin >> weapon_type;
    }

    cout << endl;
    if (weapon_type == "6")
    {
        cout << "We are sad that you decided not to buy any cookware from us. What else can I get for you?" << endl;
        //inventoryUpdate();
        cout << endl;
        return;
    }

    cout << "How many would you like? (Enter a positive integer, or 0 to cancel)" << endl; //this whole part is places outside of if causes becasue I dont want to write several times in those conditions
    string weapon;
    cin >> weapon;

    bool isInputValid = false;
    while (!isInputValid) //this while loop is fucking important!!!!!!!
    {
        while (isInputInteger(weapon)==false)
        {
            cout << "Please enter integer. or 0 to cancel" << endl;
            cin >> weapon;
        }

        if (stoi(weapon)<0 || stoi(weapon) > (5-getNumWeapon()))
        {
            cout << "Please enter a positive mulitple of 5, or 0 to cancel" << endl;
            cout << "Please enter a positive integer, or 0 to cancel" << endl;
            cout << "Note that you can only buy up to " << 5-getNumWeapon() << "  weapons becasue of limited capacity!" << endl;
            cout << "How many would you like? (Enter a positive integer, or 0 to cancel)" << endl;
            cin >> weapon;
        }
        else
        {
            isInputValid = true; //breaking the loop when the input is valid
        }
    }
    cout << endl;
    int weapon_amount;
    weapon_amount = stoi(weapon);
    if(weapon_amount==0)
    {
        cout << "We are sad that you decided not to buy any weapon from us. What else can I get for you?" << endl;
        //inventoryUpdate();
        cout << endl;
        return;
    }
   
    if (weapon_type == "1")
    {
        int cost = weapon_amount*club_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry!, you don't have enough gold for buying " << weapon_amount << " Stone Club(s)." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return; //this will be out of the function and start again att the main merchant menu
        }
        cout << "You want to buy " << weapon_amount << " Stone Club(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() - cost);
            setPartyWeaponAt(0, (getPartyWeaponAt(0))+weapon_amount);
            //inventoryUpdate();
        }
    }
    else if (weapon_type == "2")
    {
        int cost = weapon_amount*spear_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry!, you don't have enough gold for buying " << weapon_amount << " Iron Spear(s)." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return; //this will be out of the function and start again att the main merchant menu
        }
        cout << "You want to buy " << weapon_amount << " Iron Spear(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() - cost);
            setPartyWeaponAt(1,(getPartyWeaponAt(1))+weapon_amount);
            //inventoryUpdate();
        }
    }
    else if (weapon_type == "3")
    {
        int cost = weapon_amount*rapier_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry!, you don't have enough gold for buying " << weapon_amount << " Mythril Rapier(s)." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return; //this will be out of the function and start again att the main merchant menu
        }
        cout << "You want to buy " << weapon_amount << " Mythril Rapier(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() - cost);
            setPartyWeaponAt(2,(getPartyWeaponAt(2))+weapon_amount);
            //inventoryUpdate();
        }
    }
    else if (weapon_type == "4")
    {
        int cost = weapon_amount*axe_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry!, you don't have enough gold for buying " << weapon_amount << " Flaming Battle-Axe(s)." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return; //this will be out of the function and start again att the main merchant menu
        }
        cout << "You want to buy " << weapon_amount << " Flaming Battle-Axe(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() - cost);
            setPartyWeaponAt(3,(getPartyWeaponAt(3))+weapon_amount);
            //inventoryUpdate();
        }
    }
    else if (weapon_type == "5")
    {
        int cost = weapon_amount*sword_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry!, you don't have enough gold for buying " << weapon_amount << " Vorpal Longsword(s)." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return; //this will be out of the function and start again att the main merchant menu
        }
        cout << "You want to buy " << weapon_amount << " Vorpal Longsword(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() - cost);
            setPartyWeaponAt(4,(getPartyWeaponAt(4))+weapon_amount);
        }
    }
    cout << endl;
    inventoryUpdate();
    cout << endl;
    return;
}
void Dungeon::merchant_armor()
{
    //setStatusAt(0,4); //setting rooms cleared to be 2 for testing
    int armor_price = (getPartyArmorPrice()*(1+(getStatusAt(0)*0.25))); //getStatusAt(0) gives the number of room cleared
    cout << "I see that you need Armor suit! How many suits of armor can I get you? It's " << armor_price << " Gold/suit." << endl;
    cout << "(Enter a positive integer, or 0 to cancel)" << endl;
    int armor_amount;
    string armor;
    cin >> armor;
    bool isInputValid = false;
    while (!isInputValid) //this while loop is fucking important!!!!!!!
    {
        while (isInputInteger(armor)==false)
        {
            cout << "Please enter integer. or 0 to cancel" << endl;
            cin >> armor;
        }

        if (stoi(armor)<0)
        {
            cout << "Please enter a positive integer, or 0 to cancel" << endl;
            cin >> armor;
        }
        else
        {
            isInputValid = true; //breaking the loop when the input is valid
        }
    }
    cout << endl;
    armor_amount = stoi(armor);
    if (armor_amount==0)
    {
        cout << "We are sad that you decided not to buy any armor from us. What else can I get for you?" << endl;
        cout << endl;
        return; //this will be out of the function and start again att the main merchant menu
    }
    else
    {
        int cost = armor_amount*armor_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry!, you don't have enough gold for buying " << armor_amount << " armor suits" << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return; //this will be out of the function and start again att the main merchant menu
        }

        cout << "You want to buy " << armor_amount << " armor suit(s) for " << cost << " gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold((getPartyGold())-cost);
            setPartyArmor(getPartyArmor()+armor_amount);
            //inventoryUpdate();
        }
    }
    cout << endl;
    inventoryUpdate();
    cout << endl;
    return;
}
void Dungeon::merchant_sell()
{
    int num_treasures_total=0;
    for (int i=0; i<5;i++) // to calculate the total number of weapons currently held by the party
    {
        num_treasures_total = num_treasures_total + getPartyTreasureAt(i);
    }
    if (num_treasures_total==0)
    {
        cout << "You have no treasures to be sold at the moment. Please come back later!" << endl;
        cout << "What else can I get for you?" << endl;
        cout << endl;
        return;
    }

    cout << "Let me see what shiny things you have in hands! I would love to give you a great price for it!" << endl << endl;
    cout << "Here are what you have!" << endl; 
    cout << "1. " << getPartyTreasureAt(0) << " of Silver ring - " << getPartyTreasurePriceAt(0) << " gold pieces each." << endl;
    cout << "2. " << getPartyTreasureAt(1) << " Ruby necklace - " << getPartyTreasurePriceAt(1) << " gold pieces each." << endl;
    cout << "3. " << getPartyTreasureAt(2) << " Emerald bracelet - " << getPartyTreasurePriceAt(2) << " gold pieces each." << endl;
    cout << "4. " << getPartyTreasureAt(3) << " Diamond circlet - " << getPartyTreasurePriceAt(3) << " gold pieces each." << endl;
    cout << "5. " << getPartyTreasureAt(4) << " Gem-encrusted goblet - " << getPartyTreasurePriceAt(4) << " gold pieces each." << endl;
    cout << "6. Cancel" << endl;
    cout << "What do you want to sell! (Enter number between 1-6)" << endl;
    string treasure_type;
    cin >> treasure_type;
    while (!(treasure_type=="1" || treasure_type=="2" || treasure_type=="3" || treasure_type=="4" || treasure_type=="5" || treasure_type=="6"))
    {
        cout << "Please enter number between 1-6" << endl;
        cin >> treasure_type;
    }
    cout << endl;
    if (treasure_type=="6")
    {
        cout << "We are sad that you decided not to sell any treasure to us. What else can I get for you?" << endl;
        cout << endl;
        return;
    }

    cout << "How many would you like to sell? (Enter a positive integer, or 0 to cancel)" << endl; //this whole part is places outside of if causes becasue I dont want to write several times in those conditions
    int treasure_amount;
    string treasure;
    cin >> treasure_amount;
    bool isInputValid = false;
    while (!isInputValid) //this while loop is fucking important!!!!!!!
    {
        while (isInputInteger(treasure)==false)
        {
            cout << "Please enter integer. or 0 to cancel" << endl;
            cin >> treasure;
        }

        if (stoi(treasure)<0)
        {
            cout << "Please enter a positive integer, or 0 to cancel" << endl;
            cin >> treasure;
        }
        else
        {
            isInputValid = true; //breaking the loop when the input is valid
        }
    }
    cout << endl;
    treasure_amount=stoi(treasure);
    if(treasure_amount==0)
    {
        cout << "We are sad that you decided not to sell any treasure to us. What else can I get for you?" << endl;
        cout << endl;
        return;
    }

    if (treasure_type == "1")
    {
        if (treasure_amount > getPartyTreasureAt(0))
        {
            cout << "Sorry!, you don't have enough Silver ring to be sold." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return;
        }

        int cost = treasure_amount*getPartyTreasurePriceAt(0);
        cout << "You want to sell " << treasure_amount << " Silver ring(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() + cost);
            setPartyTreasureAt(0, (getPartyTreasureAt(0))-treasure_amount);
            inventoryUpdate();
        }
    }
    else if (treasure_type == "2")
    {
        if (treasure_amount > getPartyTreasureAt(1))
        {
            cout << "Sorry!, you don't have enough Ruby necklace to be sold." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return;
        }

        int cost = treasure_amount*getPartyTreasurePriceAt(1);
        cout << "You want to sell " << treasure_amount << " Ruby necklace(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() + cost);
            setPartyTreasureAt(1, getPartyTreasureAt(1)-treasure_amount);
            cout << endl;
            inventoryUpdate();
        }
    }
    else if (treasure_type == "3")
    {
        if (treasure_amount > getPartyTreasureAt(2))
        {
            cout << "Sorry!, you don't have enough Emerald bracelet to be sold." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return;
        }

        int cost = treasure_amount*getPartyTreasurePriceAt(2);
        cout << "You want to sell " << treasure_amount << " Emerald bracelet(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() + cost);
            setPartyTreasureAt(2, getPartyTreasureAt(2)-treasure_amount);
            cout << endl;
            inventoryUpdate();
        }
    }
    else if (treasure_type == "4")
    {
        if (treasure_amount > getPartyTreasureAt(3))
        {
            cout << "Sorry!, you don't have enough Diamond circlet to be sold." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return;
        }

        int cost = treasure_amount*getPartyTreasurePriceAt(3);
        cout << "You want to sell " << treasure_amount << " Diamond circlet(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() + cost);
            setPartyTreasureAt(3, getPartyTreasureAt(3)-treasure_amount);
            cout << endl;
            inventoryUpdate();
        }
    }
    else if (treasure_type == "5")
    {
        if (treasure_amount > getPartyTreasureAt(4))
        {
            cout << "Sorry!, you don't have enough Gem-encrusted goblet to be sold." << endl;
            cout << "What else can I get you?" << endl;
            cout << endl;
            return;
        }

        int cost = treasure_amount*getPartyTreasurePriceAt(4);
        cout << "You want to sell " << treasure_amount << " Gem-encrusted goblet(s) for " << cost << " Gold? (y/n)" << endl;
        string yn;
        cin >> yn;
        if(yesorno(yn))
        {
            setPartyGold(getPartyGold() + cost);
            setPartyTreasureAt(4, getPartyTreasureAt(4)-treasure_amount);
            cout << endl;
            inventoryUpdate();
        }
    }
    cout << endl;
    return;
}

void Dungeon::mainNPCAction() //done but not tested -> need to adjustthe sub function
{
    bool loop = true;
    while(loop)
    {
        printNPCAction();
        string action; //using string becasue it's easier to catch error
        cin >> action;
        while (!(action=="1" || action=="2" || action=="3"))
        {
            printNPCAction();
            cout << "Please enter number between 1-3." << endl;
            cin >> action;
        }
        cout << endl;
        if (action=="1")
        {
            //each player has a 20% chance of dropping fullness by 1
            for (int i=0; i < getNumPlayer(); i++) //looping through each player in vector
            {
                bool isDrop = probability(20); //calculating new bool for each loop -> each player has independent chance
                if (isDrop == true)
                {
                    if (getFullnessAt(i)>0)
                    {
                        setFullnessAt(i, getFullnessAt(i)-1);
                        cout << getPlayerAt(i).getName() << "'s fullness has dropped by 1." << endl;
                    }
                }
            }
            loop = false; // to break the loop and the move action will occur
        }
        else if(action=="2")
        {
            npc_speak();
        }
        else if (action=="3")
        {
            giveUp(); 
            if(getGiveup()==true) //going out as soon as giving up
            {
                cout << endl;
                return;
            }
        }
    }
    cout << endl;
    return;
}
void Dungeon::printNPCAction()
{
    cout << "You have faced NPC in this space!!! Here are things you can do! Let's see what NPC can give you!!!" << endl;
    cout << "1. Move" << endl << "2. Speak to NPC" << endl << "3. Give up" << endl;
    return;
}
void Dungeon::npc_speak()
{
    //cannot talk to NPC twice
    if (map_.getMapdata(map_.getPlayerRow(), map_.getPlayerCol()) == ' ')
    //if (map_.isExplored(map_.getPlayerRow(), map_.getPlayerCol())) //***have to check this isExplored function because NPC is set to be found when entering the space eventhough haven't talked to them
    {
        cout << "You have already talked to a NPC! Please choose another option" << endl; 
        cout << endl;
        return;
    }

    cout << "You are interacting with a NPC! Try answering their question correctly and they will be nice to you, otherwise they will summon a monster!!!" << endl;
    int riddle_index = rand()%getNumRiddle();
    string riddle_ans;
    cout << "Here is the question: " << getRiddleAt(riddle_index).getQuestion() << endl;
    cout << "What is the answer: ";
    cin >> riddle_ans;
    cout << endl;
    if(riddle_ans==getRiddleAt(riddle_index).getAnswer())
    {
        cout << "You have got the corrected answer! NPC is willing to offer trading with the merchant!" << endl << "Do you want to buy goods? (y/n)" << endl;
        string yn;
        while (!(yn == "y" || yn == "Y" || yn == "n" || yn == "N"))
        {
            cout << "Please enter 'y' to confirm that you want to trade with merchant, otherwise enter 'n'." << endl;
            cin >> yn;
        }
        if(yn == "y" || yn == "Y")
        {
            mainMerchant();
        }
        else if (yn == "n" || yn == "N")
        {
            cout << "You have lost the chance to trade with merchant." << endl;
        }
    }
    else
    {
        cout << "You have got the wrong answer? NPC has become hostile and summoned a monster!!!" << endl;
        //fighting monster function
        fightingMonster();
    }
    //moving the riddle out opf the vector
    removeRiddleAt(riddle_index);
    setNumRiddle(); // just in case

    map_.removeNPC(map_.getPlayerRow(), map_.getPlayerCol());
    //map_.exploreSpace(map_.getPlayerRow(), map_.getPlayerCol()); //setting the space to be explored
    //space will change from an NPC space to a normal space which is marked as "explored".
    cout << endl;
    return;
}

void Dungeon::mainRoom() // done but not test
{
    bool loop = true;
    while(loop)
    {
        printRoomActions();
        string action; //using string becasue it's easier to catch error
        cin >> action;
        while (!(action=="1" || action=="2" || action=="3"))
        {
            printRoomActions();
            cout << "Please enter number between 1-3." << endl;
            cin >> action;
        }
        cout << endl;
        if (action=="1") //moving
        {
            //each player has a 20% chance of dropping fullness by 1
            for (int i=0; i < getNumPlayer(); i++) //looping through each player in vector
            {
                bool isDrop = probability(20); //calculating new bool for each loop -> each player has independent chance
                if (isDrop == true)
                {
                    if (getFullnessAt(i)>0)
                    {
                        setFullnessAt(i, getFullnessAt(i)-1);
                        cout << getPlayerAt(i).getName() << "'s fullness has dropped by 1." << endl;
                    }
                }
            }
            loop = false; // to break the loop and the move action will occur at the new while loop in mainGame
        }
        else if(action=="2")
        {
            roomOpen();
        }
        else if (action=="3")
        {
            giveUp(); 
            if(getGiveup()==true) //going out as soon as giving up
            {
                cout << endl;
                return;
            }
        }
    }
    cout << endl;
    return;
}
void Dungeon::printRoomActions()
{
    cout << "You have found a secret room!!! Here are things you can do! Let's see what secret lies behind the door!!!" << endl;
    cout << "1. Move" << endl << "2. Open the door" << endl << "3. Give up" << endl;
    return;
}
void Dungeon::roomOpen()
{
    if (map_.isRoomLocation(map_.getPlayerRow(), map_.getPlayerCol()) == false)
    {
        cout << "You have already openned this room!" << endl;
        cout << endl;
        return;
    }

    bool win_puzzle = false;
    if (getStatusAt(1)==0) //trying to open the room butt does not have the key (if the player has the key this wont run and will go to the 2nd if)
    {
        cout << "You dont have a key for openning the secret room!" << endl << "You have fallen into the trap!" << endl << "To escape you must complete the puzzle!" << endl;
        //door puzzle
        cout << endl;
        cout << "The game is Boulder/Parchment/Shears! The rule is Boulder beats Shears / Shears beat Parchment / Parchment beats Boulder." << endl << "If you're lucky yours will beat the door's!!!" << endl;
        bool loop = true;
        while (loop)
        {
            cout << "To choose your guess, you have to enter: 1. Boulder   2. Shears   3.Parchment" << endl; //number assigned to the name
            int door_puzzle = rand()%3 + 1; //1-3
            string player_puzzle;
            cin >> player_puzzle;
            while (!(player_puzzle=="1" || player_puzzle=="2" || player_puzzle=="3")) //input validation
            {
                cout << "Please enter 1, 2, or 3." << endl;
                cin >> player_puzzle;
            }
            cout << endl;
            if ((player_puzzle=="1"&&door_puzzle==2) || (player_puzzle=="2"&&door_puzzle==3) || (player_puzzle=="3"&&door_puzzle==1)) // (player win)
            {
                cout << "You have won the puzzle! Now you can enter a room" << endl;
                win_puzzle=true;
                loop = false;
            }
            else if (stoi(player_puzzle)==door_puzzle) //tie
            {
                cout << "It's a tie! You have to play one more time!" << endl;
            }
            else // player loses
            {
                cout << "Sorry! You have lost the Door puzzle..." << endl;
                //losing one memeber (leader never be chosen)
                int player_index = rand()%(getNumPlayer()-1) + 1; //excluding the leader
                cout << "You have lost " << getPlayerAt(player_index).getName() << endl;
                members_.erase(members_.begin()+player_index);
                setNumPlayer(); //just in case that of using getNumPlayer later
                loop = false;
            }
        }
    }

    if (!(getNumWeapon()>0 && getPartyArmor() >0))
    {
        if (getNumWeapon()==0 && getPartyArmor()>0)
        {
            cout << "You don't have any weapon to fight a monster." << endl;
        }
        else if (getNumWeapon()>0 && getPartyArmor()==0)
        {
            cout << "You don't have any armor to protect your team fighting a monster." << endl;
        }
        else if (getNumWeapon()==0 && getPartyArmor()==0)
        {
            cout << "You don't have amy weapon and armor to fight a monster." << endl;
        }
        cout << "Please choose another option!" << endl;
        cout << endl;
        return;
    }

    //fightign monster (different than the normal fighting becasue the level is different + there is a misfortune)
    if (win_puzzle==true || getStatusAt(1)>0) // 1. dont have key to have to solve the puzzle, if win -> fall in this condition (but getStatus=0) 2. Do have a key, so win_puzzle==false but getStatus(1)>0
    {//has openned the room. R -> EXPLORED

        int mon_level = getStatusAt(0)+2; //mon_level = 1 fightign before clearing room 1,.., thus clearing 5 rooms will get mon_level = 6 which is the sorceror
        //cout << mon_level << endl;
        int mon_index;
        bool loop = true;
        while (loop)
        {
            //finding the monster
            int rand_index = rand()%getNumMonster(); //keep randomizing the index until it give the monster at that index that has the level we want
            if (getMonsterAt(rand_index).getLevel()==mon_level) //making sure that monster's level at rand_index position has the level that we want
            {
                mon_index = rand_index; //assigning rand_index into mon_index which will be used later (for getting name, level, and removing)
                loop = false;
            }
        }

        if(loop==false)
        {
            cout << getMonsterAt(mon_index).getMonName() << " AHEAD!!! THEY LOOK HOSTILE! BE CAREFUL!" << endl;
        }
        else
        {
            cout << "You have defeated all monsters of level " << mon_level << " !!!" << endl;
            cout << endl;
            return; //terminate the function as soon as there is no monster of this level left.
        }

        //fighting calculation
        int w = getNumWeapon() + getPartyWeaponAt(2) + 2*(getPartyWeaponAt(3)) + 3*(getPartyWeaponAt(4));
        int d=0;
        int r1 = rand()%6 + 1; //1-6
        int r2 = rand()%6 + 1;
        int c = mon_level;
        int a = getPartyArmor();

        //caldulating d is a problem (not sure if below code will correct -> logically)
        bool unique_weapon = true;
        for (int i=0; i<5; i++) //max num of weapon type = 5. Depending on the number of player 5 players have 5 different weapons, similary, 3 players must have 3 different weapons
        {//meaning that the number of weapon for each type has to be one for the party to have unique weapon
            if (getPartyWeaponAt(i) >1)
            {
                unique_weapon = false;
                break; //will this break out of the for loop
            }
        }
        if (unique_weapon==true)
        {
            d=4;
        }
        //if not it's 0 by the definition (already initialized to 0 at the beginning)

        //outcome condition
        int cal_outcome = ((r1*w) +d)-((r2*c)/a);
        if (cal_outcome>0) //defeating the monster
        {
            removeMonsterAt(mon_index);
            setNumMonster(); //updating just in case (it will set it by getting the length of vector of monsters)
            setNumDefeatedMonster(getNumDefeatedMonster()+1);
            cout << "You did it!!! You have defeated the monster!!!" << endl;
            //party_.setGold(party_.getGold()+(10*c));
            setPartyGold(getPartyGold() + (10*c));
            cout << "You recieve " << 10*c << " golds as a prize!" << " (" << getPartyGold() << " in total)" << endl;
            //party_.setIngredient(party_.getIngredient())
            setPartyIngredient(getPartyIngredient() + (5*c));
            cout << "You recieve " << 5*c << " ingredients as a prize!" << " (" << getPartyIngredient() << " in total)" << endl;
            bool isKey = probability(10);
            if (isKey==true)
            {
                cout << "Wait! The monster dropped the key! Lucky you!" << endl;
                setStatusAt(1, getStatusAt(1)+1); //status[1] = key
            }

            bool isMisfortune = probability(60);
            if(isMisfortune==true)
            {
                misfortuneRoom();
            }
            
        }
        else
        {//losing the monster
            //removeMonsterAt(mon_index); -> not removing bc did not win
            //setNumMonster(); //updating just in case (it will set it by getting the length of vector of monsters)
            //NOTE not adding number of defeated monstes
            cout << "The monster is too strong! You've lost the fight." << endl;
            //losing gold
            int gold_remained = getPartyGold()*0.75;
            setPartyGold(gold_remained);
            cout << "You lost some gold " << getPartyGold() << " golds are remained." << endl;
            //losing ingredient
            if (getPartyIngredient()<=30) //losing ingredient up to 30. This means that having less than 30 will have 0 remained
            {
                cout << "You have lost all the ingredient." << endl;
                setPartyIngredient(0);
            }
            else
            {
                cout << "You have lost 30 ingredients" << endl;
                setPartyIngredient(getPartyIngredient()-30); //having more than 30 will be substracted by 30
            }
            //losing member
            for (int i=1; i<getNumPlayer(); i++) //i=1 for excluding the party leader
            {
                if(getPartyArmor() > 0) //5% chance
                {
                    bool isSlain = probability(5);
                    if(isSlain==true)
                    {
                        cout << getPlayerAt(i).getName() << " was killed by the monster!" << endl;
                        members_.erase(members_.begin()+i);
                        setNumPlayer(); //just in case that of using getNumPlayer later
                    }
                }
                else //10% chance
                {
                    bool isSlain = probability(10);
                    if(isSlain==true)
                    {
                        cout << getPlayerAt(i).getName() << " was killed by the monster!" << endl;
                        members_.erase(members_.begin()+i);
                        setNumPlayer(); //just in case that of using getNumPlayer later
                    }
                }
            }
            //removePlayer(); //not die by hunger -> removing the player right away
            
            //40% chance misfortune occur
            bool isMisfortune = probability(40);
            if (isMisfortune==true)
            {
                misfortuneRoom();
            }
        }

        if (getStatusAt(1)>0) //openning the room by the key
        {
            setStatusAt(1, getStatusAt(1)-1);
        }
        //using remove Room
        map_.removeRoom(map_.getPlayerRow(),map_.getPlayerCol());
        //setting the number of room openned(cleared)
        setStatusAt(0, getStatusAt(0)+1);
    }
    else //getStatus(1)==0 (dont have a key) and win_puzzle==false (lost the puzzle)
    {
        cout << "You have lost the puzzle and does not have a key! Please come back later!" << endl;
    }
    cout << endl;
    return;
}

void Dungeon::misfortuneRoom() //done but not test
{
    int random = rand()%10+1;
    if (random == 1) //losing random weapon or armor
    {

        int rand1 = rand()%2;
        if (rand1 == 0) //losing weapon
        {
            if (getNumWeapon()==0) //no weapon
            {
                cout << "Good for you, you have no weapon to be broken!" << endl;
                return;
            }
            //use the while loop to random the index of weapon to find the one that is not 0
            bool weapon_found = false;
            int weapon_index =0; //ginvign the initial value does not matter (be cause this loop will always find one, this is the case that num_weapon is not zero)
            while (!weapon_found) //while it's true (weapon_not found yet)
            {
                int index_rand = rand()%5; //0-4
                if (getPartyWeaponAt(index_rand)!=0)
                {
                    weapon_index=index_rand;
                    weapon_found=true;
                }
            }
            if (weapon_index==0)
            {
                cout << "OH NO! Your Club broke!" << endl;
            }
            else if (weapon_index==1)
            {
                cout << "OH NO! Your Spear broke!" << endl;
            }
            else if (weapon_index==2)
            {
                cout << "OH NO! Your +1 Rapier broke!" << endl;
            }
            else if (weapon_index==3)
            {
                cout << "OH NO! Your +2 Battle-Axe broke!" << endl;
            }
            else if (weapon_index==4)
            {
                cout << "OH NO! Your +3 Longsword broke!" << endl;
            }
            setPartyWeaponAt(weapon_index, getPartyWeaponAt(weapon_index)-1);
            }
            else //losing armor
            {
                if (getPartyArmor()==0)
                {
                    cout << "Good for you! You have no armor to be broken!" << endl;
                    return;
                }
                cout << "OH NO! Your Armoe broke!" << endl;
                setPartyArmor(getPartyArmor()-1);
            }   
    }
    else if (random>1&&random<5) // 2,3,4
    {
        if (random == 2)//losing ingredient
        {
            if (getPartyIngredient()==0)
            {
                cout << "Good for you! You have no ingredient to be stolen!" << endl;
                return;
            }
            else if (getPartyIngredient()<=10)
            {
                cout << "OH NO! Your team was robbed by dungeon bandits!" << endl;
                cout << "Your " << getPartyIngredient() << " kg of ingredient got stolen!" << endl;
                setPartyIngredient(0);
            }
            else if (getPartyIngredient()>10)  
            {
                cout << "OH NO! Your team was robbed by dungeon bandits!" << endl;
                cout << "Your 10 kg of ingredient got stolen!" << endl;
                setPartyIngredient(getPartyIngredient()-10);
            }
        }
        else if(random ==3) //losing cookware
        {
            if (getNumCookware()==0)
            {
                cout << "Good for you! You have no cookware to be stolen!" << endl;
                return;
            }
            bool cookware_found = false;
            int cookware_index =0;
            while (!cookware_found)
            {
                int index_rand = rand()%5; //0-4
                if (getPartyCookwareAt(index_rand)!=0)
                {
                    cookware_index=index_rand;
                    cookware_found=true;
                }
            }
            cout << "OH NO! Your team was robbed by dungeon bandits!" << endl;
            if (cookware_index==0)
            {
                cout << "OH NO! Your Ceramic Pot got stolen!" << endl;
            }
            else if (cookware_index==1)
            {
                cout << "OH NO! Your Frying Pan got stolen!" << endl;
            }
            else if (cookware_index==2)
            {
                cout << "OH NO! Your Cauldron got stolen!" << endl;
            }
            setPartyCookwareAt(cookware_index, getPartyCookwareAt(cookware_index)-1);
        }
        else if (random == 4)
        {
            if (getPartyArmor()==0)
            {
                cout << "Good for you! You have no armor to be stolen!" << endl;
                return;
            }
            cout << "OH NO! Your team was robbed by dungeon bandits!" << endl;
            cout << "OH NO! Your lost 1 armor!" << endl;
            setPartyArmor(getPartyArmor()-1);
        }
    }
    else if (random>4 && random<8) // 5,6,7
    {
        int player_index = rand()%getNumPlayer();
        if (getFullnessAt(player_index)<=10)
        {
            if (player_index!=0) //not the leader
            {
                cout << getPlayerAt(player_index).getName() << "'s fullness has dropped to 0 and die from hunger. (because of the misfortune)" << endl;
                members_.erase(members_.begin()+player_index);
                setNumPlayer(); //just in case that of using getNumPlayer later
            }
            else
            {
                cout << "The leader " << getPlayerAt(0).getName() << "'s fullness has dropped to 0. The game is over!" << endl;
                setGiveup(true);
                return;
            }
            //setFullnessAt(player_index, 0);
        }
        else if (getFullnessAt(player_index)>10)
        {
            setFullnessAt(player_index, getFullnessAt(player_index)-10);
            cout << "There is a food poisoning! "<< getPlayerAt(player_index).getName() << "'s fullness has dropped to " << getFullnessAt(player_index) << ". (because of the misfortune)" << endl;
        }
    }
    else if (random>7 && random<11) //8,9,10
    {
        //player locked in the previous room
        //occur only when the party tries to open the door
        int player_index = rand()%(getNumPlayer()-1) + 1;
        cout << "OH NO! Your teammate " << getPlayerAt(player_index).getName() << " is trapped in the previous room and is unable to get through. You must continue without them." << endl;
        members_.erase(members_.begin()+player_index);
        setNumPlayer(); //just in case that of using getNumPlayer later
        cout << "Your party size has reduced to " << getNumPlayer() << " members." << endl;

        //size is not enough (leader+1 member)
        if (getNumPlayer()<2)
        {
            //using giveup_ as a by part helper
            setGiveup(true);
            return;
        }

    }
    return;
}
void Dungeon::misfortuneNormal() //done but not tested
{
    int random = rand()%10+1;
    if (random == 1) //losing random weapon or armor
    {

        int rand1 = rand()%2;
        if (rand1 == 0) //losing weapon
        {
            if (getNumWeapon()==0) //no weapon
            {
                cout << "Good for you, you have no weapon to be broken!" << endl;
                return;
            }
            //use the while loop to random the index of weapon to find the one that is not 0
            bool weapon_found = false;
            int weapon_index =0; //ginvign the initial value does not matter (be cause this loop will always find one, this is the case that num_weapon is not zero)
            while (!weapon_found) //while it's true (weapon_not found yet)
            {
                int index_rand = rand()%5; //0-4
                if (getPartyWeaponAt(index_rand)!=0)
                {
                    weapon_index=index_rand;
                    weapon_found=true;
                }
            }
            if (weapon_index==0)
            {
                cout << "OH NO! Your Club broke!" << endl;
            }
            else if (weapon_index==1)
            {
                cout << "OH NO! Your Spear broke!" << endl;
            }
            else if (weapon_index==2)
            {
                cout << "OH NO! Your +1 Rapier broke!" << endl;
            }
            else if (weapon_index==3)
            {
                cout << "OH NO! Your +2 Battle-Axe broke!" << endl;
            }
            else if (weapon_index==4)
            {
                cout << "OH NO! Your +3 Longsword broke!" << endl;
            }
            setPartyWeaponAt(weapon_index, getPartyWeaponAt(weapon_index)-1);
            }
            else //losing armor
            {
                if (getPartyArmor()==0)
                {
                    cout << "Good for you! You have no armor to be broken!" << endl;
                    return;
                }
                cout << "OH NO! Your Armoe broke!" << endl;
                setPartyArmor(getPartyArmor()-1);
            }   
    }
    else if (random>1&&random<5) // 2,3,4
    {
        if (random == 2)//losing ingredient
        {
            if (getPartyIngredient()==0)
            {
                cout << "Good for you! You have no ingredient to be stolen!" << endl;
                return;
            }
            else if (getPartyIngredient()<=10)
            {
                cout << "OH NO! Your team was robbed by dungeon bandits!" << endl;
                cout << "Your " << getPartyIngredient() << " kg of ingredient got stolen!" << endl;
                setPartyIngredient(0);
            }
            else if (getPartyIngredient()>10)  
            {
                cout << "OH NO! Your team was robbed by dungeon bandits!" << endl;
                cout << "Your 10 kg of ingredient got stolen!" << endl;
                setPartyIngredient(getPartyIngredient()-10);
            }
        }
        else if(random ==3) //losing cookware
        {
            if (getNumCookware()==0)
            {
                cout << "Good for you! You have no cookware to be stolen!" << endl;
                return;
            }
            bool cookware_found = false;
            int cookware_index =0;
            while (!cookware_found)
            {
                int index_rand = rand()%5; //0-4
                if (getPartyCookwareAt(index_rand)!=0)
                {
                    cookware_index=index_rand;
                    cookware_found=true;
                }
            }
            cout << "OH NO! Your team was robbed by dungeon bandits!" << endl;
            if (cookware_index==0)
            {
                cout << "OH NO! Your Ceramic Pot got stolen!" << endl;
            }
            else if (cookware_index==1)
            {
                cout << "OH NO! Your Frying Pan got stolen!" << endl;
            }
            else if (cookware_index==2)
            {
                cout << "OH NO! Your Cauldron got stolen!" << endl;
            }
            setPartyCookwareAt(cookware_index, getPartyCookwareAt(cookware_index)-1);
        }
        else if (random == 4)
        {
            if (getPartyArmor()==0)
            {
                cout << "Good for you! You have no armor to be stolen!" << endl;
                return;
            }
            cout << "OH NO! Your team was robbed by dungeon bandits!" << endl;
            cout << "OH NO! Your lost 1 armor!" << endl;
            setPartyArmor(getPartyArmor()-1);
        }
    }
    else if (random>4 && random<8) // 5,6,7
    {
        int player_index = rand()%getNumPlayer();
        if (getFullnessAt(player_index)<=10)
        {
            if (player_index!=0) //not the leader
            {
                members_.erase(members_.begin()+player_index);
                setNumPlayer(); //just in case that of using getNumPlayer later
                cout << getPlayerAt(player_index).getName() << "'s fullness has dropped to 0 and die from hunger. (because of the misfortune)" << endl;
            }
            else
            {
                cout << "The leader " << getPlayerAt(0).getName() << "'s fullness has dropped to 0. The game is over!" << endl;
                setGiveup(true);
                return;
            }
            //setFullnessAt(player_index, 0);
        }
        else if (getFullnessAt(player_index)>10)
        {
            setFullnessAt(player_index, getFullnessAt(player_index)-10);
            cout << "There is a food poisoning! "<< getPlayerAt(player_index).getName() << "'s fullness has dropped to " << getFullnessAt(player_index) << ". (because of the misfortune)" << endl;
        }
    }
    return;
}

void Dungeon::mainGame()
{
    // isEndgame is defaulted as false
    while(!isEndgame()) // !isEndgame()=true when the game continues (loop continues), !isEndgame()=false when game ends (loop stops)
    {
        statusUpdate(); //-> including party and inventory
        cout << endl;
        map_.displayMap();
        cout << endl;
        bool isHunWarn = false;
        if(hungerWarning()) //if there is a hunger warning at the start -> player's fullness that is still 0 at the end of the turn will be eliminate
        {
            isHunWarn = true;
        }
        
        if (map_.getMapdata(map_.getPlayerRow(), map_.getPlayerCol()) == ' ')
        //if (map_.isExplored(map_.getPlayerRow(), map_.getPlayerCol()))
        {
            cout << "You have entered an explored space." << endl;
        }
        else if(map_.isFreeSpace(map_.getPlayerRow(), map_.getPlayerCol())) //player's position == normal space (free space)
        {//CONDITION WORK
            cout << "You've enter a free space." << endl;
            mainNormalSpace();
        }
        else if(map_.isRoomLocation(map_.getPlayerRow(), map_.getPlayerCol())) // player's position == room
        {//CONDITION WORKS
            //cout << "You have found a room" << endl;
            mainRoom();
        }
        else if (map_.getMapdata(map_.getPlayerRow(), map_.getPlayerCol()) == 'N')
        //else if (map_.isNPCLocation(map_.getPlayerRow(), map_.getPlayerCol())) // player's position = NPC space
        { //this statement does not work when entering NPC because MOVE function from the map will set NPC space = explored space and 
            cout << "You have found a NPC" << endl;
            mainNPCAction();
        }
        else if(map_.isDungeonExit(map_.getPlayerRow(), map_.getPlayerCol())) //CONDITION WORKS
        {
            cout << "You have found the Dungeon exist" << endl;
            return;
        }

        if (isEndgame()) //checking before moving
        {
            return;
        }

        if (map_.isRoomLocation(map_.getPlayerRow(), map_.getPlayerCol())) //two conditions fro separating misfortuneNormal and Room
        {
            moveAction();
            if(isHunWarn)
            {
                for (int i=0; i<getNumPlayer(); i++)
                {
                    if (getFullnessAt(i)==0)
                    {
                        cout << "OH NO! " << getPlayerAt(i).getName() << " has died from hunger!" << endl;
                        members_.erase(members_.begin()+i);
                        setNumPlayer(); //just in case that of using getNumPlayer later
                        cout << "Your party size has reduced to " << getNumPlayer() << " members." << endl;
                    }
                }
            }
            misfortuneRoom();
        }
        else
        {
            moveAction();
            if(isHunWarn)
            {
                for (int i=0; i<getNumPlayer(); i++)
                {
                    if (getFullnessAt(i)<=0)
                    {
                        cout << "OH NO! " << getPlayerAt(i).getName() << " has died from hunger!" << endl;
                        members_.erase(members_.begin()+i);
                        setNumPlayer(); //just in case that of using getNumPlayer later
                        cout << "Your party size has reduced to " << getNumPlayer() << " members." << endl;
                    }
                }
            }
            misfortuneNormal();
        }
    }
    //cout << "Entering ending phase!" << endl;

    return;
}
void Dungeon::endGame() ////almost done but not tested
{
    cout << "THE JOURNEY HAS ENDED!!!" << endl;
    if (getStatusAt(0)==5 && getNumPlayer()>1 && (map_.getPlayerCol()==map_.getDungeonExitCol() && map_.getPlayerRow()==map_.getDungeonExitRow())) //winning condition
    {
        cout << "CONGRATULATION!!! You have successfully escaped the dungeon!!!" << endl;
    }
    else
    {
        cout << "You have lost in the dungeon forever! No light will ever shine upon you for the eternity!" << endl;
    }

    //cout << "Here is you final status!" << endl;
    //name of the leader + remaining members
    partyUpdate();
    //number of rooms cleared
    cout << "The number of cleared room: " << getStatusAt(0) << endl;
    //gold pieces remaining
    cout << "The amount of remained Gold pieces: " << getPartyGold() << endl;
    //treasure items (using loop to print only the one that does not have 0 amount)
    cout << "The treasures currently collected and held: ";
    for (int i=0; i<5; i++)
    {
        // if(getPartyTreasureAt(i)!=0)
        // {
            if (i==0)
            {
                cout << "Silver Ring - " << getPartyTreasureAt(0) << " | ";
            }
            else if (i==1)
            {
                cout << "Ruby necklace - " << getPartyTreasureAt(1) << " | ";
            }
            else if (i==2)
            {
                cout << "Emerald bracelet - " << getPartyTreasureAt(2) << " | ";
            }
            else if (i==3)
            {
                cout << "Diamond circlet - " << getPartyTreasureAt(3) << " | ";
            }
            else if (i==4)
            {
                cout << "Gem-encrusted goblet - " << getPartyTreasureAt(4) << " | ";
            }
        // }
    }
    cout << endl;

    //number of space explored -> possibly have to create variable keep tracking the explored space
    cout << "The number of explored spaces: " << getNumExplored() << endl;
    //number of monster defeated
    cout << "The number of monsters defeated: " << getNumDefeatedMonster() << endl;
    //number of turns elapsed
    cout << "The number of turns taken: " << getStatusAt(3) << endl;
    return;

    //save all these data in "results.txt"
}
double Dungeon::scoreCalculation() //tested
{
    int room_cleared = getStatusAt(0);

    double room_factor = 1 + (room_cleared*0.5);
    double total_score = 0.0;
    double inventory_score = 0;
    double party_score = 0;
    double winning_score = 0;
    //selling everything back into money (inventory, tresures) -> based on the room cleared
    inventory_score = getPartyGold() + getPartyIngredient()*getPartyIngredientPrice()*room_factor + getPartyArmor()*getPartyArmorPrice()*room_factor;
    for (int i=0; i<3; i++) //cookware
    {
        inventory_score += getPartyCookwareAt(i)*getPartyCookwarePriceAt(i)*room_factor;
    }
    for (int i=0; i<5; i++) //weapon
    {
        inventory_score += getPartyWeaponAt(i)*getPartyWeaponPriceAt(i)*room_factor;
    }
    for (int i=0; i<5; i++)
    {
        inventory_score += getPartyTreasureAt(i)*getPartyTreasurePriceAt(i); //this one does not th increased factor becasue the type/price itself already depends on the number of rooms cleared
    }
    //the number of members () -> based on room cleared + their fullness level
    party_score = getNumPlayer()*30*room_factor;
    for (int i=0; i< getNumPlayer(); i++)
    {
        party_score =+ getFullnessAt(i)*room_factor;
    }
    //the number of defeated  monsters -> based on room cleared
    double monsters_score = getNumDefeatedMonster()*20*room_factor;
    //number of turn -> based on room cleared
    double turns_score = (100.0-getStatusAt(3))*2*room_factor; //the more steps the less score
    //anger level
    double anger_score = (100.0-getStatusAt(2))*3*room_factor;

    //plus 300 point if finding the dungeon exist
    if (getStatusAt(0)==5 && getNumPlayer()>1 && (map_.getPlayerCol()==map_.getDungeonExitCol() && map_.getPlayerRow()==map_.getDungeonExitRow()))
    {
        winning_score = 300;
    }
    total_score = inventory_score + party_score + monsters_score + turns_score + anger_score + winning_score - 650; //starting score is 650 -> probably have to normalize to be 0

    //cout << "Your total score is: " << total_score << " points." << endl;

    return total_score;
    //save total_score and leader's name in "score board.txt"
}
void Dungeon::saveStats() //tested
{
    ofstream fout;
    string filename = "stats.txt";
    fout.open(filename, ios::app);
    //Party members
    for (int i=0; i<getNumPlayer(); i++)
    {
        fout << getPlayerAt(i).getName() << ": " << getFullnessAt(i) << endl;
    }
    //number of rooms cleared
    fout << "The number of cleared room: " << getStatusAt(0) << endl;
    //gold pieces remaining
    fout << "The amount of remained Gold pieces: " << getPartyGold() << endl;
    //treasure items (using loop to print only the one that does not have 0 amount)
    fout << "The treasures currently collected and held: ";
    for (int i=0; i<5; i++)
    {
        if(getPartyTreasureAt(i)!=0)
        {
            if (i==0)
            {
                fout << "Silver Ring - " << getPartyTreasureAt(0) << " | ";
            }
            else if (i==1)
            {
                fout << "Ruby necklace - " << getPartyTreasureAt(1) << " | ";
            }
            else if (i==2)
            {
                fout << "Emerald bracelet - " << getPartyTreasureAt(2) << " | ";
            }
            else if (i==3)
            {
                fout << "Diamond circlet - " << getPartyTreasureAt(3) << " | ";
            }
            else if (i==4)
            {
                fout << "Gem-encrusted goblet - " << getPartyTreasureAt(4) << " | ";
            }
        }
    }
    fout << endl; 

    //number of space explored -> possibly have to create variable keep tracking the explored space
    fout << "The number of explored spaces: " << getNumExplored() << endl;
    //number of monster defeated
    fout << "The number of monsters defeated: " << getNumDefeatedMonster() << endl;
    //number of turns elapsed
    fout << "The number of turns taken: " << getStatusAt(3) << endl;
    fout << endl;
    fout.close();
    return;
}
void Dungeon::saveScore() //tested
{
    ofstream fout;
    string filename = "score.txt";
    fout.open(filename, ios::app);
    
    fout << getPlayerAt(0).getName() << ":" << scoreCalculation() << endl;
    fout.close();
    return;
}
void Dungeon::scoreBoard() //tested
{
    //reading from score.txt
    ifstream fin;
    string filename = "score.txt";
    fin.open(filename);

    vector<vector<string>> name_score;
    string line;
    vector<string> temp_array(2); //(name,score)
    char separater = ':';
    while (!fin.eof())
    {
        getline(fin, line);
        //split function
        if (line.length()!=0) //ignoring the empty line
        {
            for (int i=0; i<line.length(); i++)
            {
                if (line[i]==separater)
                {
                    temp_array.at(0)=line.substr(0,i); //starting at 0 and stop in front of i(separater)
                    temp_array.at(1)=line.substr(i+1); //cutting after the separator til the end of the line
                    break;
                }
            }
            if (name_score.size()==0)
            {
                name_score.push_back(temp_array); //storing the frist array (line) into the vector
            }
            else
            {
                //comparing -> passed!!!!
                for (int i=0; i<name_score.size(); i++)
                {//as long as the temp score is less than the one from the vector, keep moving temp to the right ***stop when the vector's is less than temp's
                    if (stoi(temp_array.at(1)) > stoi(name_score.at(i).at(1))) //name_score[i][1] might give an error
                    {
                        //inserting array into the vector
                        name_score.insert(name_score.begin()+i,temp_array); //inserting the temp array at i index -> in front of number that less than temp's
                        break;
                    }
                }
            }
        }
    }//after this while loop -> should have populated vector sorted from highest to the lowest
    cout << "SCORE BOARD" << endl;
    for (int i=0; i< name_score.size(); i++)//using for loop to cout the scoreboard
    {
        cout << i+1 << " " << name_score[i][0] << ": " << name_score[i][1] << " points." << endl;
    }
    fin.close();
    return;
}