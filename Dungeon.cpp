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
#include <cctype> 
#include "Dungeon.h"
#include <ctime> 
#include <cstdlib>
#include <math.h>

using namespace std;
// DEFAULT INITIALIZER //
Dungeon::Dungeon() 
{
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

// STARTING THE GAME //
/*
Algorithm will start the game!
1. The map of the game will first be displayed with player's origin set, add NPC randomly throughout map, 
and add rooms.
2. User will be asked input 5 names to populate the members vector - party will be updated
3. Will print out prompt where menu will soon be printed for userto purchase items before entering main game
5. Inventory will be printed so user can see their inventory 
6. Monsters and Riddles will be populated before main game as well
*/
void Dungeon::start() 
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

    //populating vector of monster
    string mon_filename = "monsters.txt";
    readMonster(mon_filename);
    //populating vector of riddles
    string rid_filename = "riddles.txt";
    readRiddle(rid_filename);
    //cout << getNumRiddle() << endl;
    cout << endl;
    return;
}

// MEMBERS //
/*
Algorithm will add player 
1. pass Player through function
2. add player to vector
3. update num of player count
*/
void Dungeon::addPlayer(Player player)
{
    members_.push_back(player);
    num_player_++;
}
/*
Algorithm will get player at certain index in vector
1. pass specific integer i
2. return the specific player at that index
*/
Player Dungeon::getPlayerAt(int i)
{
    return members_.at(i);
}
// Algorithm will return number of players
int Dungeon::getNumPlayer()
{
    return num_player_;
}
// Algorithm will set the number of players 
void Dungeon::setNumPlayer()
{
    num_player_ = members_.size();
}
/*
Algorithm will get player at certain index and set its fullness to certain integer
1. Utilize vector of players 
2. get certainplayer at certain index
3. set new fullness
*/
void Dungeon::setFullnessAt(int player_index, int new_fullness)
{
    vector<Player> &members = members_;
    members.at(player_index).setFullness(new_fullness);
    return;
}
/*
Algorithm will get fullness of player at index
1. get playey at certain index 
2. get fullness from Player class
*/
int Dungeon::getFullnessAt(int player_index)
{
    return getPlayerAt(player_index).getFullness();
}
/*
Algorithm will remove player if fullness is at 0 or less
1. will loop through each player and determine fullness
2. if fullness is at 0, will erase player from that index 
3. will subtract count of number of players to be updated
*/
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
/*
Algorithm will remove weapon to ensure that it is never greater than the number of players since 
each player can only have one weapon
1. while weapon is greater than number of players
2. continue to randomly select a weapon to remove until it is less than or equal to
3. print out which weapon the user will be losing in game
*/
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
/*
Algorithm will remove armor from inventory
1. determine if the number of armor in inventory is greater than number of player since each player can only 
have one
2. will set the number of armor the same as number of player
*/
void Dungeon::removeArmor()
{
    if (getPartyArmor() > getNumPlayer())
    {
        setPartyArmor(getNumPlayer());
    }
    return;
}

// PARTY //
// SETTERS //

/*
Algorithm will update the number of certain cookware at its specific index
1. pass the index and value through function
2. set that specific cookware inventory, its new value
*/
void Dungeon::setPartyCookwareAt(int index, int value)
{
    party_.setCookwareAt(index, value);
    return;
}
// Algorithm will set cookware prices at certain index its new price
void Dungeon::setPartyCookwarePriceAt(int index, int value)
{
    party_.setCookwarePriceAt(index, value);
    return;
}
// Algorithm will update the number of a specific weapon at its specific index
void Dungeon::setPartyWeaponAt(int index, int value)
{
    party_.setWeaponAt(index, value); 
    return;
}
// Algorithm will set weapon prices at certain index its new price
void Dungeon::setPartyWeaponPriceAt(int index, int value)
{
    party_.setWeaponPriceAt(index, value); 
    return;
}
// Algorithm will update the number of the specific treasure found at certain index
void Dungeon::setPartyTreasureAt(int index, int value)
{
    party_.setTreasureAt(index, value);
    return;
}
// Algorithm will set treasure prices at certain index its new price
void Dungeon::setPartyTreasurePriceAt(int index, int value)
{
    party_.setTreasurePriceAt(index, value);
    return;
}
// Algorithm will set the number of gold
void Dungeon::setPartyGold(int value)
{
    party_.setGold(value);
}
// Algorithm will set the amount of ingredients
void Dungeon::setPartyIngredient(int value)
{
    party_.setIngredient(value);
}
// Algorithm will set the price of the ingredients
void Dungeon::setPartyIngredientPrice(int value)
{
    party_.setIngredientPrice(value);
}
// Algorithm will set the number of armor there is
void Dungeon::setPartyArmor(int value)
{
    party_.setArmor(value);
}
// Algorithm will set the price of armor
void Dungeon::setPartyArmorPrice(int value)
{
    party_.setArmorPrice(value);
}

// GETTERS //
// Algorithm will return number of specific cookware at its index
int Dungeon::getPartyCookwareAt(int index)
{
    return party_.getCookwareAt(index);
}
// Algorithm will return the price of specific cookware at its index
int Dungeon::getPartyCookwarePriceAt(int index)
{
    return party_.getCookwarePriceAt(index);
}
// Algorithm will return the number of specific weapon at its index
int Dungeon::getPartyWeaponAt(int index)
{
    return party_.getWeaponAt(index);
}
// Algorithm will return the price of specific weapon at its index
int Dungeon::getPartyWeaponPriceAt(int index)
{
    return party_.getWeaponPriceAt(index);
}
// Algorithm will return the number of specific treasure at its index
int Dungeon::getPartyTreasureAt(int index)
{
    return party_.getTreasureAt(index);
}
// Algorithm will return the price of specific treasure at its index
int Dungeon::getPartyTreasurePriceAt(int index)
{
    return party_.getTreasurePriceAt(index);
}
// Algorithm will return number of gold
int Dungeon::getPartyGold()
{
    return party_.getGold();
}
// Algorithm will return number of ingredients
int Dungeon::getPartyIngredient()
{
    return party_.getIngredient();
}
// Algorithm will return price of ingredients
int Dungeon::getPartyIngredientPrice()
{
    return party_.getIngredientPrice();
}
// Algoritm will return number of armor
int Dungeon::getPartyArmor()
{
    return party_.getArmor();
}
// Algorithm will return price of armor
int Dungeon::getPartyArmorPrice()
{
    return party_.getArmorPrice();
}
/*
Algorithm will calculate the number of weapons the party currently holds - 5 is the max
1. Will loop through each index to get each number of weapons at its specific index
2. Will then add to the number of weapons total 
3. return total of weapons
*/
int Dungeon::getNumWeapon()
{
    int num_weapons_total=0;
    for (int i=0; i<5;i++) // to calculate the total number of weapons currently held by the party
    {
        num_weapons_total += getPartyWeaponAt(i);
    }
    return num_weapons_total;
}
/*
Algorithm will calculate the number of cookware the party currently holds 
1. Will loop through each index to get each number of coookware at its specific index
2. Will then add to the number of cookware total 
3. return total of cookware
*/
int Dungeon::getNumCookware()
{
    int num_cookware=0;
    for (int i=0; i<3; i++)
    {
        num_cookware += getPartyCookwareAt(i);
    }
    return num_cookware;
}

// MONSTER //
// Algorithm will return monster at certain index
Monster Dungeon::getMonsterAt(int i)
{
    return mons_.at(i);
}
/*
Algorithm will read file of monsters and levels that will then be placed in the monster vector
1. will pass in the filename that is provided called monsters.txt
2. open file that will be read
3. will split name and level of monster 
4. will be placed into the monster vector
5. update count of monster
6. close file
*/
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
/*
Algorithm will remove monster at certain index
1. will accept integer parameter that will be the index
2. remove the monster at that specific location in vector
3. update monster count
*/
void Dungeon::removeMonsterAt(int i)
{
    mons_.erase(mons_.begin()+i);
    num_mons_--;
    return;
}
// Algorithm will return the number of monsters
int Dungeon::getNumMonster()
{
    return num_mons_;
}
// Algoritm will set the number of monsters
void Dungeon::setNumMonster()
{
    num_mons_ = mons_.size();
    return;
}
// Algorithm will return the number of monsters defeated
int Dungeon::getNumDefeatedMonster()
{
    return num_mons_defeated_;
}
// Algorithm will set the number of monsters defeated 
void Dungeon::setNumDefeatedMonster(int num_mons_defeated)
{
    num_mons_defeated_ = num_mons_defeated;
    return;
}
/*
Algorithm will determine if monsters at a specific level is present
1. will pass monster level into function
2. Loop through each monster and get its level
3. if level is present, haveMon will become true and break out of loop
*/
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

// RIDDLES //
/*
Algorithm will read file of questions and answers that will then be placed in the riddles vector
1. will pass in the filename that is provided called riddles.txt
2. open file that will be read
3. will split question and answers
4. will be placed into the riddles vector
5. update count of riddles
6. close file
*/
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
// Algorithm will return riddle at certain index
Riddle Dungeon::getRiddleAt(int i)
{
    return riddles_.at(i);
}
// Algorithm will return the number of riddles
int Dungeon::getNumRiddle()
{
    return num_riddles_;
}
// Algoritm will set the number of riddles
void Dungeon::setNumRiddle()
{
    num_riddles_ = riddles_.size();
}
/*
Algorithm will remove riddle at certain index
1. will accept integer parameter that will be the index
2. remove the riddle at that specific location in vector
3. update riddle count
*/
void Dungeon::removeRiddleAt(int i)
{
    riddles_.erase(riddles_.begin()+i);
}

// STATUS //
// Algorithm will return the status at certain index
int Dungeon::getStatusAt(int index)
{
    return status_[index];
}
// Algorithm will set the status at certain index a specific value
void Dungeon::setStatusAt(int index, int value)
{
    status_[index] = value;
}

// MAP //
Map Dungeon::getMap()
{
    return map_;
}

// GIVE UP //
bool Dungeon::getGiveup()
{
    return giveup_;
}
void Dungeon::setGiveup(bool giveup)
{
    giveup_ = giveup;
}

// OTHER FUNCTIONS //
/*
Algorithm will check if user string input is an integer
1. will pass in string into function
2. will loop through input length
3. if the string can't be converted to integer (STOI), isValid = false and break
4. if it can be converted, it will return true
*/
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
// Algorithm will return the whole inventory status that player currently has in game
void Dungeon::inventoryUpdate() 
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
// Algorithm will return each player's name and their fullness
void Dungeon::partyUpdate() 
{
    cout << "PARTY | " << endl;
    for (int i=0; i < getNumPlayer(); i++)
    {
        cout << getPlayerAt(i).getName() << " | " << "Fullness: " << getPlayerAt(i).getFullness() << endl; 
    }
    return;
}
/*
Algorithm will calculate the probability of an event occuring
1. will pass prob_percent through the function
2. function will calculate a random number
3. determine if the random number is less than or greater than the prob_percent
4. If it is less than, the even will return true otherwise it will be false
*/
bool Dungeon::probability(int prob_percent)
{
    int random = rand()%100 + 1; //giving number between 1-100
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
/*
Algorithm will return true or false depending on yes or no answers from user input when purchasing items
1. will accept string into function
2. will return true if yes
3. will return false if no
*/
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
// Algorithm will print out status along with inventory and party
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
/*
Algorithm will return true for possibilities of how game can end 
1. will return true if only one player is left
2. will return true if anger level of sorcerer is 100
3. will return true if all 5 rooms have been cleared and reached exit
4. will return true if user gives up
5. check happens every action or move that is done in game
*/
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
/*
Algorithm will show instructions for moving each space 
1. accept user input 
2. will check whether it is out of bounds - if out of bounds, will ask user again 
3. move user if in bound and correct char is used
4. increment turns and anger level 
*/
void Dungeon::moveAction() 
{
    bool loop = false;
    while (!loop)
    {
        cout << "Let's move to another space!!!" << endl << "Here are the options for making a move!" << endl << "'w' to move up" << endl << "'s' to move down" << endl << "'a' to move left" << endl << "'d' to move rightt" << endl;
        char move_input; //give error easily using char -> using string is easier but have to implement move function
        cin >> move_input;
        while (!(move_input=='w' || move_input=='W' || move_input=='s' || move_input=='S' || move_input=='a' || move_input=='A' || move_input=='d' || move_input=='D') || cin.fail()) //***fail in case of inputting string "clear", it will go through the loop 3 time before reading 'a' as option for moving
        {
            cin.clear();
            cin.ignore(500,'\n'); 
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
            {
                setStatusAt(3, getStatusAt(3)+1); //+1 number of turns when the new space is not already expored
            }

            //increment anger level (for every single move)
            setStatusAt(2, getStatusAt(2)+1);
            loop = true;
        }
    }
    return;
}
/*
Algorithm will determine if party can fight mosnter, if so, will calculate if they win or not
1. if user doesn't have any armor or weapons, they can't fight the monster
2. will determine level of monster party can fight dependong on rooms cleared
3. will let user know if they defeated all monsters of certain level
4. will choose random monster of level so they can fight
5. calculate the fighting score using formula 
6. if they win, they receive gold, ingredients, and may win a key
7. if they lose, they lose gold and ingredients. They may also lose a teammate depending on the # of armor they have
8. if player dies, armor and weapon will be updated
*/
void Dungeon::fightingMonster() 
{
    //validation check for the number of weapon and armor
    if (!(getNumWeapon()>0 && getPartyArmor() >0))
    {
        if (getNumWeapon()==0 && getPartyArmor()>0)
        {
            cout << "You don't have any weapons to fight the monster." << endl;
        }
        else if (getNumWeapon()>0 && getPartyArmor()==0)
        {
            cout << "You don't have any armor to protect your team w henfighting the monster." << endl;
        }
        else if (getNumWeapon()==0 && getPartyArmor()==0)
        {
            cout << "You don't have any weapons or armor to fight the monster!" << endl;
        }
        cout << "Please choose another option!" << endl;
        return;
    }

    int mon_level = getStatusAt(0)+1; //mon_level = 1 fightign before clearing room 1,.., thus clearing 5 rooms will get mon_level = 6 which is the sorceror
    if(!ishaveMonLevel(mon_level)) //does not have mon
    {
        cout << "You have defeated all the monsters of level " << mon_level << " !!!" << endl;
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
    if (cal_outcome>0)
    {
        removeMonsterAt(mon_index); //removing the monster only in case when u win it
        setNumMonster(); //updating just in case (it will set it by getting the length of vector of monsters)
        setNumDefeatedMonster(getNumDefeatedMonster()+1);
        cout << "You did it!!! You have defeated the monster!!!" << endl;
        //party_.setGold(party_.getGold()+(10*c));
        setPartyGold(getPartyGold() + (10*c));
        cout << "You recieved " << 10*c << " gold as a prize!" << " (" << getPartyGold() << " in total)" << endl;
        //party_.setIngredient(party_.getIngredient())
        setPartyIngredient(getPartyIngredient() + (5*c));
        cout << "You recieved " << 5*c << " kg ingredients as a prize!" << " (" << getPartyIngredient() << " in total)" << endl;
        bool isKey = probability(10);
        if (isKey==true)
        {
            cout << "Wait! The monster dropped a key! Lucky you!" << endl;
            setStatusAt(1, getStatusAt(1)+1); //status[1] = key
            cout << "You currently have " << getStatusAt(1) << " key(s)" << endl;
        }
    }
    else
    {
        //not removing monster
        //NOTE not adding number of defeated monsters
        cout << "The monster is too strong! You've lost the fight." << endl;
        //losing gold
        int gold_remained = getPartyGold()*0.75;
        setPartyGold(gold_remained);
        cout << "You lost some gold " << getPartyGold() << " golds are remained." << endl;
        //losing ingredient
        if (getPartyIngredient()<=30) //losing ingredient up to 30. This means that having less than 30 will have 0 remained
        {
            cout << "You have lost all the your ingredients." << endl;
            setPartyIngredient(0);
        }
        else
        {
            cout << "You have lost 30 kg ingredients" << endl;
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
                    removeWeapon();
                    setNumPlayer(); //just in case that of using getNumPlayer later
                    removeArmor();
                }
            }
            else //10% chance
            {
                bool isSlain = probability(10);
                if(isSlain==true)
                {
                    cout << getPlayerAt(i).getName() << " was killed by the monster!" << endl;
                    members_.erase(members_.begin()+i);
                    removeWeapon();
                    setNumPlayer(); //just in case that of using getNumPlayer later
                    removeArmor();
                }
            }
        }
        removeWeapon(); //always checks after losing player
    }
    return;
}
/*
Algorithm will determine if user really wants to give up
1. Ask user if they are sure they want to give up - takes in user input
2. if user inputs y/Y, will call setGiveup function and set as true to end game
3. if user inputs n/N, game will continue
*/
void Dungeon::giveUp() 
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
        cout << "I knew you still had courage left in you! Let's escape the dungeon!!!" << endl;
    }
    return;
}
/*
Algorithm occurs at the beginning of each turns and return warning if any player is on the verge of death due to starvation
1. loop through each player and check their fullness
2. if fullness is = to 1, it will warn user
3. iswarning will be true
4. will return true so user is aware their teammate is on the verge of death
*/
bool Dungeon::hungerWarning() //this one has to be at the beginning of each turns -> using bool becasue it will connect to whether you gonna erase player (dying by hunger) at the end or not
{
    bool isWarning = false;
    for (int i=0; i<getNumPlayer(); i++)
    {
        if (getFullnessAt(i)<=1)
        {
            cout << "Warning: " << getPlayerAt(i).getName() << " is on the blink of starvation!" << endl << "You should cook and eat some food now!!" << endl;
            isWarning = true;
        }
    }
    return isWarning;
}
// Algorithm will return the number of spaces explored
int Dungeon::getNumExplored() 
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
/*
Algorithm occurs when user has reached the final battle against the sorcerer
1. if there are less than 3 players, user will receive a companion to fight if they can answer the riddle correctly
2. will receive a chance to purcahse items from merchant before battle
3. there will be three phases when fighting the sorcerer
4. phase 1 is where the sorcerer's health is greater than 100 - probability of attack is 20 and of finding potion is 10
5. phase 2 is where the sorcerer's health is less than 100 but greater than 50 - probability of attack is 25 and of finding potion is 20
6. phase 3 is where the sorcerer's health is less than 50 - probability of attack is 30 and of finding potion is 30
7. if leader dies, user loses or if there are less than 2 players
8. will check each player's fullness to determine if any teammates have been lost
9. once sorcerer dies, the party can escape the dungeon
*/
void Dungeon::fightingSorcerer() 
{
    cout << "You have come to the last boss SORCERER!!! This battle will determine the fate of your party!!!" << endl;
    cout << endl;

    //giving the desription of fighting sorceror
    cout << "The Sorcerer is stronger than normal monsters you have fought. Sorcerer has fullness at 150 which is separated into 3 phases, 50 each." << endl;
    cout << "The Sorcerer will be angrier and more difficult to defeat each phase you have entered!" << endl;
    cout << endl;
    //getting pet monster to be part of your team (same as player)
    if (getNumPlayer()<3)
    {
        cout << "Lucky for you! You have found a monster pet (Vega) that can help you fight the Sorcerer!" << endl << "However, to catch them, you have answer their question!" << endl;
        int riddle_index = rand()%getNumRiddle();
        string riddle_ans;
        cout << "Here is the question: " << getRiddleAt(riddle_index).getQuestion() << endl;
        cout << "What is the answer: ";
        cin >> riddle_ans;
        cout << endl;
        if(riddle_ans==getRiddleAt(riddle_index).getAnswer())
        {
            cout << "You got the corrected answer! Vega is willing to help you fight and be part of your team!" << endl;
            cout << endl;
            Player mon_pet("Vega");
            addPlayer(mon_pet);
            partyUpdate();
            cout << endl;
        }
        else
        {
            cout << "You got the wrong answer? You have lost Vega!!!" << endl;
        }
        //moving the riddle out opf the vector
        removeRiddleAt(riddle_index);
        setNumRiddle(); // just in case
    }

    cout << "You have one last chance to prepare you team for this legendary battle! Gather as many items as possible to increase your fate of survival!!" << endl;
    mainMerchant();
    cout << endl;

    int sorcerer_fullness = 150;
    while (sorcerer_fullness>0 && getNumPlayer()>1 && getFullnessAt(0)>0)
    {
        if (sorcerer_fullness>=100) //phase 1
        {
            cout << "Phase 1" << endl;
            //calculating attacking score for the party
            int damage = attackingScore();
            cout << "You have done " << damage << " points of damage to the Sorcerer!" << endl;
            sorcerer_fullness -= damage;
            cout << endl;
            //probability of attack again
            if (probability(30))
            {
                cout << "You got luck and can double attack!!!" << endl;
                int double_damage = attackingScore();
                cout << "You have done " << double_damage << " points of damage to the sorcerer!" << endl;
                sorcerer_fullness -= double_damage;
            }
            cout << endl;

            //calculating damage doe by sorceror to the party
            int rand_damage = rand()%3+1; //specific to this phase (1-3)
            cout << "The Sorcerer has fought back!!! Your party's fullness has decreased by " << rand_damage << endl;
            cout << endl;
            for (int i=0; i<getNumPlayer(); i++)
            {
                if (getFullnessAt(i)>rand_damage)
                {
                    setFullnessAt(i,getFullnessAt(i)-rand_damage);
                }
                else
                {
                    setFullnessAt(i,0);
                }
            }

            //findingn potion for increasing fullness
            if(probability(10))
            {
                int rand_healing = rand()%3+1;
                cout << "You have found a potion that helps increase your party's fullness by " << rand_healing << endl;
                for (int i=0; i<getNumPlayer(); i++)
                {
                    setFullnessAt(i, getFullnessAt(i)+rand_healing);
                }
            }
        }
        else if (sorcerer_fullness>=50) // phase2
        {
            cout << "Phase 2" << endl;
            //calculating attacking score for the party
            int damage = attackingScore();
            cout << "You have done " << damage << " points of damage to the Sorcerer!" << endl;
            sorcerer_fullness -= damage;
    
            cout << endl;
            //probability of attack again
            if (probability(25) && sorcerer_fullness!=0)
            {
                cout << "You got lucky and can double attack!!!" << endl;
                int double_damage = attackingScore();
                cout << "You have done " << double_damage << " points of damage to the Sorcerer!" << endl;
                sorcerer_fullness -= double_damage;
            }
            cout << endl;

            //calculating damage doe by sorceror to the party
            int rand_damage = rand()%3+3; //specific to this phase (3-5)
            cout << "The Sorcerer has fought back!!! Your party's fullness has decreased by " << rand_damage << endl;
            cout << endl;
            for (int i=0; i<getNumPlayer(); i++)
            {
                if (getFullnessAt(i)>rand_damage)
                {
                    setFullnessAt(i,getFullnessAt(i)-rand_damage);
                }
                else
                {
                    setFullnessAt(i,0);
                }
            }

            //findingn potion for increasing fullness
            if(probability(20))
            {
                int rand_healing = rand()%3+1;
                cout << "You have found a potion that helps increase your party's fullness by " << rand_healing << endl;
                for (int i=0; i<getNumPlayer(); i++)
                {
                    setFullnessAt(i, getFullnessAt(i)+rand_healing);
                }
            }
        }
        else if (sorcerer_fullness>=0) //phase 3
        {
            cout << "Phase 3" << endl;
            //calculating attacking score for the party
            int damage = attackingScore();
            cout << "You have done " << damage << " points of damage to the Sorcerer!" << endl;
            if(sorcerer_fullness>damage)
            {
                sorcerer_fullness -= damage;
            }
            else
            {
                sorcerer_fullness = 0;
                break;
            }

            cout << endl;
            //probability of attack again
            if (probability(20))
            {
                cout << "You got lucky and can double attack!!!" << endl;
                int double_damage = attackingScore();
                cout << "You have done " << double_damage << " points of damage to the Sorcerer!" << endl;
                if(sorcerer_fullness>double_damage)
                {
                    sorcerer_fullness -= double_damage;
                }
                else
                {
                    sorcerer_fullness = 0;
                    break;
                }
            }
            cout << endl;

            //calculating damage doe by sorceror to the party
            int rand_damage = rand()%3+5; //specific to this phase (5-7)
            cout << "The Sorcerer has fought back!!! Your party's fullness has decreased by " << rand_damage << endl;
            cout << endl;
            for (int i=0; i<getNumPlayer(); i++)
            {
                if (getFullnessAt(i)>rand_damage)
                {
                    setFullnessAt(i,getFullnessAt(i)-rand_damage);
                }
                else
                {
                    setFullnessAt(i,0);
                }
            }

            //findingn potion for increasing fullness
            if(probability(30))
            {
                int rand_healing = rand()%3+1;
                cout << "You have found a potion that helps increase your party's fullness by " << rand_healing << endl;
                for (int i=0; i<getNumPlayer(); i++)
                {
                    setFullnessAt(i, getFullnessAt(i)+rand_healing);
                }
            }
        }

        if (getFullnessAt(0)<=0) //leader fullness reaches 0
        {
            cout << "You have lost the party leader. The game is over!!!" << endl;
            setGiveup(true);
            cout << endl;
            return;
        }

        for (int i=0; i<getNumPlayer(); i++)
        {
            if (getFullnessAt(i)<=0)
            {
                cout << "You have lost " << getPlayerAt(i).getName() << " after the battle." << endl;
                members_.erase(members_.begin()+i);
                removeWeapon();
                setNumPlayer();
                removeArmor();
            }
        }
        //party / sorcerer update
        partyUpdate();
        cout << "Sorcerer: " << sorcerer_fullness << endl;
        cout << endl;
    }

    //finding sorcerer index in the vector
    int sorcerer_index;
    for (int i=0; i<getNumMonster(); i++)
    {
        if (getMonsterAt(i).getLevel()==6)
        {
            sorcerer_index = i;
            break;
        }
    }

    if (sorcerer_fullness>0 || getFullnessAt(0)==0 || getNumPlayer()<2) //meaning that the party has lost
    {
        cout << "The Socerer is too strong! You've lost the fight." << endl;
        //losing gold
        cout << endl;
        int gold_remained = getPartyGold()*0.75;
        setPartyGold(gold_remained);
        cout << "You lost some gold " << getPartyGold() << " gold remain." << endl;
        //losing ingredient
        if (getPartyIngredient()<=30) //losing ingredient up to 30. This means that having less than 30 will have 0 remained
        {
            cout << "You have lost all your ingredients." << endl;
            setPartyIngredient(0);
        }
        else
        {
            cout << "You have lost 30 kg of ingredients" << endl;
            setPartyIngredient(getPartyIngredient()-30); //having more than 30 will be substracted by 30
        }
    }
    else
    {
        int c =6; //level of sorcerer
        cout << "You have defeated the Sorcerer! It's time to head out for the dungeon exist!" << endl;
        removeMonsterAt(sorcerer_index); //*****
        setNumMonster(); //updating just in case (it will set it by getting the length of vector of monsters)
        setNumDefeatedMonster(getNumDefeatedMonster()+1);
        setPartyGold(getPartyGold() + (10*c));
        cout << "You recieved " << 10*c << " gold as a prize!" << " (" << getPartyGold() << " in total)" << endl;
        setPartyIngredient(getPartyIngredient() + (5*c));
        cout << "You recieved " << 5*c << " kg of ingredients as a prize!" << " (" << getPartyIngredient() << " in total)" << endl;
    }
    
    cout << endl;
    return;
}
// NORMAL //
/*
Algorithm will show menu of what user can do (move, investigate, cook/eat, pick a fight, or give up)
1. accept user input as string
2. check string with isInputInteger function
3. if false, ask for user input again
4. otherwise, will convert user option to integer and go to option user has chosen
5. every move, user has 20% chance of losing fullness by 1
*/
int Dungeon::attackingScore()
{
    //fighting calculation
    int w = getNumWeapon() + getPartyWeaponAt(2) + 2*(getPartyWeaponAt(3)) + 3*(getPartyWeaponAt(4));
    int d=0;
    int r1 = rand()%6 + 1; //1-6
    int r2 = rand()%6 + 1;
    int c = 6;
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
    return cal_outcome;
}
/*
Algorithm will show menu of what user can do (move, investigate, cook/eat, pick a fight, or give up)
1. accept user input as string
2. check string with isInputInteger function
3. if false, ask for user input again
4. otherwise, will convert user option to integer and go to option user has chosen
5. every move, user has 20% chance of losing fullness by 1
*/
void Dungeon::mainNormalSpace()
{
    bool loop = true;
    while(loop)
    {
        printNormalSpaceActions();
        int main_choice;
        string main;
        cin >> main;
        cout << endl;

        bool isInputValid = false;
        while (!isInputValid) 
        {
            while (isInputInteger(main)==false)
            {
                cout << "Please enter an integer." << endl;
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
/*
Algorithm will print out options user can do such as move, investigate, pick a fight, cook/eat, or give up
*/
void Dungeon::printNormalSpaceActions()
{
    cout << "Here are things you can do! Choose wisely and the odds may be with you!" << endl;
    cout << "1. Move" << endl << "2. Investigate" << endl << "3. Pick a fight" << endl << "4. Cook and Eat" << endl << "5. Give up" << endl;
    return;
}
/*
Algorithm will determine the probability of the user finding a key, running into a monster, if they find a treasure/
how close they are to the treasure, and mark the space as explored on the map
1. if user doesn't have any armor or weapons, they can't fight the monster
2. check if space has already been explored, if so they move again
3. if they find a key, it will let user know
4. let user know if they have found treasure or if they are close 
5. let user know if they encountered a monster or not and ask user if they want to fight it, if they do run into one
6. space will be marked as explored
*/
void Dungeon::normalSpaceInvestigate() 
{
    if(!(map_.isExplored(map_.getPlayerRow(), map_.getPlayerCol()))) //if it's not explored
    {//dont forget to set this space to be explored at the end
        bool isKeyFound = probability(10);
        if (isKeyFound==true)
        {
            cout << "You have found a key for the secret room!!! Keep it tight in your hand and try not to get it stolen!!!" << endl;
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
            cout << "You have found a hostile monster!!! You may choose either to fight or surrender!!!" << endl;
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
                cout << "Monster is too strong! It's ok to surrender. Your party will win next time!!!" << endl;
                int index = rand()%(getNumPlayer()-1) + 1; //gonna give the range that exclude 0. For example, if num_player=5 -> rand()%4+1 will be 1-4.
                cout  << getPlayerAt(index).getName() << " has been held captive by the monster! You must continue without them!" << endl;
                members_.erase(members_.begin()+index);
                setNumPlayer(); //just in case that of using getNumPlayer later
                removeWeapon();
                removeArmor();
            }
            //after facing monster -> 50% chance that all players' fullness decrease by 1
            bool isFullnessDecrease = probability(50);
            if (isFullnessDecrease==true)
            {
                cout << "Your party's fullness has dropped by 1. (after fighting a monster)" << endl;
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
            cout << "Good for you! There is no monster in this space!" << endl;
            bool isFullnessDecrease = probability(50);
            if (isFullnessDecrease==true)
            {
                cout << "Your party's fullness has dropped by 1. (after investigating a space.)" << endl;
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
    cout << endl;
    return;
}
/*
Algorithm will determine if user can cook/eat, if so, will use ingredients/cookware from user input and 
calculate party's new fullness
1. if there is no cookware/ingredients, user can not cook/eat
2. otherwise, will ask use how much they want to cook
3. user isInputInteger function to determine if it's valid
4. if not, user will be prompted again.
5. if input is 0, user doesn't want to cook
6. ask user for cookware and will determine if they have that specific one
7. once they have the # of ingredients and the cookware, food will be calculated and will be evenly
distributed to all party members
8. user will get to see the updated fullness, ingredients, and cookware (if it breaks or not)
*/
void Dungeon::normalSpaceCookeat() 
{
    //ingredient != 0 && numCookware != 0
    if (getPartyIngredient()<=0&&getNumCookware()<=0)
    {
        cout << "We're so sorry, you don't have any ingredients or cookware for making food!" << endl;
        cout << endl;
        return;
    }
    else if (getPartyIngredient()>0&&getNumCookware()<=0)
    {
        cout << "You have some ingredients! Sadly, you don't have any cookware to use." << endl;
        cout << endl;
        return;
    }
    else if (getPartyIngredient()<=0&&getNumCookware()>0)
    {
        cout << "You have some cookware! Sadly, you don't have any ingredients to use." << endl;
        cout << endl;
        return;
    }
    // need both in order to cook

    //prompting user to choose amount of ingredient
    cout << "You have " << getPartyIngredient() << " kg of ingredients. The ratio of successful cooking is 1 fullness: 5 kg." << endl << "How much do you want to cook?" << endl;
    int amount_ing;
    string ing;
    cin >> ing; 

    bool isInputValid = false;
        while (!isInputValid) 
        {
            while (isInputInteger(ing)==false)
            {
                cout << "Please enter an integer." << endl;
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
    string cw;
    while (not_pass)
    {
        cout << "Here is the list of cookware types and ingredients avaiable to you!!" << endl;
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
            not_pass = false; 
        }
    }

    int cw_index = stoi(cw)-1;
    if (cw=="1")
    {
        bool isBreak = probability(25);
        if (isBreak==true)
        {
            cout << "NOOOOO! a ceramic pot has broken! You have lost it and you lost " << amount_ing << " kg of ingredients." << endl;
            setPartyCookwareAt(cw_index, getPartyCookwareAt(cw_index)-1);//losing cookware
            setPartyIngredient(getPartyIngredient()-amount_ing);//losing ingredient
        }
        else
        {
            cout << "You have successfully cooked " << amount_ing << " kg of ingredients!" << endl << "everyone's fullness has increased by " << amount_ing/5 << endl;
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
            cout << "NOOOOO! a frying has broken! You have lost it and you lost " << amount_ing << " kg of ingredients." << endl;
            setPartyCookwareAt(cw_index, getPartyCookwareAt(cw_index)-1);//losing cookware
            setPartyIngredient(getPartyIngredient()-amount_ing);//losing ingredient
        }
        else
        {
            cout << "You have successfully cooked " << amount_ing << " kg of ingredients!" << endl << "everyone's fullness has increased by " << amount_ing/5 << endl;
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
            cout << "NOOOOO! a frying has broken! You have lost it and you lost " << amount_ing << " kg of ingredients." << endl;
            setPartyCookwareAt(cw_index, getPartyCookwareAt(cw_index)-1);//losing cookware
            setPartyIngredient(getPartyIngredient()-amount_ing);//losing ingredient
        }
        else
        {
            cout << "You have successfully cooked " << amount_ing << " kg of ingredients!" << endl << "everyone's fullness has increased by " << amount_ing/5 << endl;
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
    cout << "You have " << getPartyIngredient() << " kg of ingredients left." << endl;
    cout << endl;
    return;
}
/*
Algorithm will determine if party can fight mosnter, if so, will calculate if they win or not
1. if user doesn't have any armor or weapons, they can't fight the monster
2. will determine level of monster party can fight dependong on rooms cleared
3. will let user know if they defeated all monsters of certain level
4. will choose random monster of level so they can fight
5. calculate the fighting score using formula 
6. if they win, they receive gold, ingredients, and may win a key
7. if they lose, they lose gold and ingredients. They may also lose a teammate depending on the # of armor they have
8. each player will also have a 50% chance of losing fullness by 1
9. if player dies, armor and weapon will be updated
*/
void Dungeon::normalSpacePickfight() 
{
    //validation check for the number of weapon and armor
    if (!(getNumWeapon()>0 && getPartyArmor() >0))
    {
        if (getNumWeapon()==0 && getPartyArmor()>0)
        {
            cout << "You don't have any weapons to fight a monster." << endl;
        }
        else if (getNumWeapon()>0 && getPartyArmor()==0)
        {
            cout << "You don't have any armor to protect your team fighting the monster." << endl;
        }
        else if (getNumWeapon()==0 && getPartyArmor()==0)
        {
            cout << "You don't have any weapons or armor to fight the monster." << endl;
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
    if (cal_outcome>0)
    {
        removeMonsterAt(mon_index); //removing the monster only in case when u win it
        setNumMonster(); //updating just in case (it will set it by getting the length of vector of monsters)
        setNumDefeatedMonster(getNumDefeatedMonster()+1);
        cout << "You did it!!! You have defeated the monster!!!" << endl;
        //party_.setGold(party_.getGold()+(10*c));
        setPartyGold(getPartyGold() + (10*c));
        cout << "You recieved " << 10*c << " gold as a prize!" << " (" << getPartyGold() << " in total)" << endl;
        //party_.setIngredient(party_.getIngredient())
        setPartyIngredient(getPartyIngredient() + (5*c));
        cout << "You recieved " << 5*c << " kg of ingredients as a prize!" << " (" << getPartyIngredient() << " in total)" << endl;
        bool isKey = probability(10);
        if (isKey==true)
        {
            cout << "Wait! The monster dropped a key! Lucky you!" << endl;
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
        cout << "You lost some gold " << getPartyGold() << " gold are remain." << endl;
        //losing ingredient
        if (getPartyIngredient()<=30) //losing ingredient up to 30. This means that having less than 30 will have 0 remained
        {
            cout << "You have lost all your ingredients." << endl;
            setPartyIngredient(0);
        }
        else
        {
            cout << "You have lost 30 kg ingredients" << endl;
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
                    removeArmor();
                }
            }
            else //10% chance
            {
                bool isSlain = probability(10);
                if(isSlain==true)
                {
                    cout << getPlayerAt(i).getName() << " was killed by the monster!" << endl;
                    members_.erase(members_.begin()+i);
                    removeWeapon();
                    setNumPlayer(); //just in case that of using getNumPlayer later
                    removeArmor();
                }
            }
        }
        if(probability(50))//50% chance everyone fullness drops by 1 
        {
            cout << "Your party's fullness has dropped by one." << endl;
            partyUpdate();
            cout << getNumPlayer() << endl;
            for (int i=0; i<getNumPlayer(); i++) 
            {
                if (getFullnessAt(i)>0)
                {
                    setFullnessAt(i, getFullnessAt(i)-1);
                }
            }
            cout << endl;
            partyUpdate();
        }
        removeWeapon(); //always checks after losing player
    }
    cout << endl;
    return;
}
// MERCHANT //
/*
Algorithm will show menu of what user can buy or if user wants to sell treasures
1. us isInputInteger function to determine if user input is valid 
2. if not, prompt user again
3. otherwise it will be converted to integer and will go into option that user chooses
4. once user chooses 6, it will break out of the function and be user will be done buying
*/
void Dungeon::mainMerchant()
{
    bool loop = true;
    while (loop)
    {
        printMerchant();
        int main_choice;
        string main;
        cin >> main;

        bool isInputValid = false;
        while (!isInputValid) 
        {
            while (isInputInteger(main)==false)
            {
                cout << "Please enter an integer." << endl;
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
// Algorithm will print out what user can buyor if user wants to sell treasures - user leaves once finished
void Dungeon::printMerchant()
{
    cout << "Let's make some modifications to your party!!! Here are your option! Be prepared for a long journey!!!" << endl;
    cout << "1. Ingredients: To make food, you have to cook raw ingredients." << endl;
    cout << "2. Cookware: You will need something to cook those ingredients." << endl;
    cout << "3. Weapons: It's dangerous to go alone, take this!" << endl;
    cout << "4. Armor: If you want to survive monster attacks, you will need some armor." << endl;
    cout << "5. Sell treasures: If you find anything shiny, I would be happy to take it off your hands." << endl;
    cout << "6. Leave: Make sure you get everything you need, I'm leaving after this sale!" << endl;
    return;
}
/*
Algorithm will ask user the amount of ingredients desired and will determine if it's valid or not
1. ask user for amount they want
2. determine if string can be converted to integer 
3. determine if input is a multiple of 5 and positive, if not prompt again
4. if user inputs 0, return
5. determine if user has enough gold 
6. ask user if they are sure with their purchase and show cost
7. update gold and ingredients for status 
*/
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
            cout << "Please enter an integer." << endl;
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
        cout << "We are sad that you decided not to buy any ingredients from us. What else can I do for you?" << endl;
        cout << endl;
        return; //this will be out of the function and start again att the main merchant menu
    }
    else
    {
        int cost = ingre_amount*ingredient_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry! You don't have enough gold for buying " << ingre_amount << " kg of ingredients" << endl;
            cout << "What else can I do you?" << endl;
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
/*
Algorithm will ask user the type of cookware desired and will ask for amount to sell to user
1. ask user for type they want and amount
2. determine if string can be converted to integer for both prompts of type and amount
3. determine if input is a multiple of 5 and positive, if not prompt again
4. if user inputs 0, return
5. determine if user has enough gold 
6. ask user if they are sure with their purchase and show cost
7. update gold and cookware for status 
*/
void Dungeon::merchant_cookware()
{
    int pot_price = (getPartyCookwarePriceAt(0)*(1+(getStatusAt(0)*0.25))); //we dont want based price (data member) to change
    int pan_price = (getPartyCookwarePriceAt(1)*(1+(getStatusAt(0)*0.25)));
    int cauldron_price = (getPartyCookwarePriceAt(2)*(1+(getStatusAt(0)*0.25)));

    cout << "I have several types of cookware, which one would you like?" << endl << "Each type has a different probability of breaking when used, marked with (XX%)." << endl;
    cout << "Choose on of the following cokwares:" << endl;
    cout << "1. (25%) Ceramic Pot [" << pot_price << " Gold]" << endl;
    cout << "2. (10%) Frying Pan [" << pan_price << " Gold]" << endl;
    cout << "3. ( 2%) Cauldron [" << cauldron_price << " Gold]" << endl;
    cout << "4. Cancel" << endl;
    string cw_type;
    cin >> cw_type;
    while (!(cw_type == "1" || cw_type == "2" || cw_type == "3" || cw_type == "4")) //connsider the length becasue it gives error for white space -> does nto work
    {
        cout << "Please enter a number between 1-4" << endl;
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
            cout << "Please enter an integer. or 0 to cencel" << endl;
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
        cout << "We are sad that you decided not to buy any cookware from us. What else can I do for you?" << endl;
        cout << endl;
        return;
    }
    
    if (cw_type=="1")
    {
        int cost = cook_amount*pot_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry! You don't have enough gold for buying " << cook_amount << " Ceremic Pot(s)." << endl;
            cout << "What else can I do you?" << endl;
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
            cout << "Sorry! You don't have enough gold for buying " << cook_amount << " Frying Pan(s)." << endl;
            cout << "What else can I do you?" << endl;
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
            cout << "Sorry! You don't have enough gold for buying " << cook_amount << " Cauldron(s)." << endl;
            cout << "What else can I do you?" << endl;
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
/*
Algorithm will ask user the type of weapon desired and will ask for amount to sell to user
1. ask user for type they want and amount
2. determine if string can be converted to integer for both prompts of type and amount
3. determine if input is a positive integer and not over num of players, if not prompt again
4. if user inputs 0, return
5. determine if user has enough gold 
6. ask user if they are sure with their purchase and show cost
7. update gold and weapons for status 
*/
void Dungeon::merchant_weapon()
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
    cout << "I have a wide collection of weapons to choose from, what would you like?" << endl << "Note that some of them provide you a special bonus in combat, marked by a (+X)." << endl;
    cout << "Each player can hold only one weapon! Your party can have up too 5 weapons!" << endl;
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
        cout << "Please enter a number between 1-6" << endl;
        cin >> weapon_type;
    }

    cout << endl;
    if (weapon_type == "6")
    {
        cout << "We are sad that you decided not to buy any weapon from us. What else can I do for you?" << endl;
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
            cout << "Please enter an integer. or 0 to cancel" << endl;
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
        cout << "We are sad that you decided not to buy any weapon from us. What else can I do for you?" << endl;
        //inventoryUpdate();
        cout << endl;
        return;
    }
   
    if (weapon_type == "1")
    {
        int cost = weapon_amount*club_price;
        if (cost > getPartyGold())
        {
            cout << "Sorry! You don't have enough gold for buying " << weapon_amount << " Stone Club(s)." << endl;
            cout << "What else can I do you?" << endl;
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
            cout << "Sorry! You don't have enough gold for buying " << weapon_amount << " Iron Spear(s)." << endl;
            cout << "What else can I do you?" << endl;
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
            cout << "Sorry! You don't have enough gold for buying " << weapon_amount << " Mythril Rapier(s)." << endl;
            cout << "What else can I do you?" << endl;
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
            cout << "Sorry! You don't have enough gold for buying " << weapon_amount << " Flaming Battle-Axe(s)." << endl;
            cout << "What else can I do you?" << endl;
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
            cout << "Sorry! You don't have enough gold for buying " << weapon_amount << " Vorpal Longsword(s)." << endl;
            cout << "What else can I do you?" << endl;
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
/*
Algorithm will ask user the number of armor desired and will determine if it's valid or not
1. ask user for amount they want
2. determine if string can be converted to integer 
3. determine if input is a positive integer and not over num of players, if not prompt again
4. if user inputs 0, return
5. determine if user has enough gold 
6. ask user if they are sure with their purchase and show cost
7. update gold and ingredients for status 
*/
void Dungeon::merchant_armor()
{
    //setStatusAt(0,4); //setting rooms cleared to be 2 for testing
    int armor_price = (getPartyArmorPrice()*(1+(getStatusAt(0)*0.25))); //getStatusAt(0) gives the number of room cleared
    cout << "I see that you need Armor! How many suits of armor can I get you? It's " << armor_price << " Gold/suit." << endl;
    cout << "(Enter a positive integer, or 0 to cancel)" << endl;
    int armor_amount;
    string armor;
    cin >> armor;
    bool isInputValid = false;
    while (!isInputValid) //this while loop is fucking important!!!!!!!
    {
        while (isInputInteger(armor)==false)
        {
            cout << "Please enter an integer. or 0 to cancel" << endl;
            cin >> armor;
        }

        if (stoi(armor)<0 || stoi(armor) > (getNumPlayer()-getPartyArmor()))
        {
            cout << "Please enter a positive integer, or 0 to cancel" << endl;
            cout << "Note that you can only buy up to " << getNumPlayer()-getPartyArmor() << "  weapons becasue of limited capacity!" << endl;
            cout << "How many would you like? (Enter a positive integer, or 0 to cancel)" << endl;
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
            cout << "Sorry! You don't have enough gold for buying " << armor_amount << " armor suits" << endl;
            cout << "What else can I do you?" << endl;
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
/*
Algorithm will determine if user has any treasures to sell, if they do, will continue to prompt and calculate amount they will receive
1. if treasure amount user has is 0, return
2. if not, ask user which type of treasure they want to sell
3. ask user how which type and determine if its valid input
4. ask user how many and determine if its valid input
5. calculate the type they chose adn confirm if user still wants to sell
6. once done, update inventory 
*/
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
    cin >> treasure;
    bool isInputValid = false;
    while (!isInputValid) //this while loop is fucking important!!!!!!!
    {
        while (isInputInteger(treasure)==false)
        {
            cout << "Please enter an integer. or 0 to cancel" << endl;
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
    treasure_amount = stoi(treasure);
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
            cout << "Sorry! You don't have enough Silver ring to be sold." << endl;
            cout << "What else can I do you?" << endl;
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
            cout << "Sorry! You don't have enough Ruby necklace to be sold." << endl;
            cout << "What else can I do you?" << endl;
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
            cout << "Sorry! You don't have enough Emerald bracelet to be sold." << endl;
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
            cout << "Sorry! You don't have enough Diamond circlet to be sold." << endl;
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
            cout << "Sorry! You don't have enough Gem-encrusted goblet to be sold." << endl;
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
                removeWeapon();
                removeArmor();
                loop = false;
            }
        }
    }

    if (!(getNumWeapon()>0 && getPartyArmor() >0))
    {
        if (getNumWeapon()==0 && getPartyArmor()>0)
        {
            cout << "You don't have any weapons to fight a monster." << endl;
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
    if ((win_puzzle==true || getStatusAt(1)>0) && getStatusAt(0)<4) // 1. dont have key to have to solve the puzzle, if win -> fall in this condition (but getStatus=0) 2. Do have a key, so win_puzzle==false but getStatus(1)>0
    {

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
                        removeArmor();
                    }
                }
                else //10% chance
                {
                    bool isSlain = probability(10);
                    if(isSlain==true)
                    {
                        cout << getPlayerAt(i).getName() << " was killed by the monster!" << endl;
                        members_.erase(members_.begin()+i);
                        removeWeapon();
                        setNumPlayer(); //just in case that of using getNumPlayer later
                        removeArmor();
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
    else if ((win_puzzle==true || getStatusAt(1)>0) && getStatusAt(0)>=4)
    {
        fightingSorcerer();
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
                removeWeapon();
                setNumPlayer(); //just in case that of using getNumPlayer later
                removeArmor();
            }
            else
            {
                cout << "The leader " << getPlayerAt(0).getName() << "'s fullness has dropped to 0. The game is over!" << endl;
                setFullnessAt(0,0);
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
        removeWeapon();
        setNumPlayer(); //just in case that of using getNumPlayer later
        removeArmor();
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
                removeWeapon();
                setNumPlayer(); //just in case that of using getNumPlayer later
                removeArmor();
                cout << getPlayerAt(player_index).getName() << "'s fullness has dropped to 0 and die from hunger. (because of the misfortune)" << endl;
            }
            else
            {
                cout << "The leader " << getPlayerAt(0).getName() << "'s fullness has dropped to 0. The game is over!" << endl;
                setFullnessAt(0, 0);
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
                        removeWeapon();
                        setNumPlayer(); //just in case that of using getNumPlayer later
                        removeArmor();
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
                        removeWeapon();
                        setNumPlayer(); //just in case that of using getNumPlayer later
                        removeArmor();
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
