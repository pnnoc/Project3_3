// Map.h
// CSCI 1300 Fall 2022
// Author: Con Muangkod
// Recitation: 100 – Keshavaiah Naidu
// Project3
// Partner: Thanh-Thanh Dao
#ifndef MAP_H
#define MAP_H

#include <iostream> 

using namespace std;

class Map
{
private:
    const char UNEXPLORED = '-'; // marker for unexplored spaces
    const char EXPLORED = ' ';   // marker for explored spaces
    const char ROOM = 'R';       // marker for room locations
    const char NPC = 'N';        // marker for NPC locations
    const char PARTY = 'X';      // marker for party position
    const char EXIT = 'E';       // marker for dungeon exit

    static const int num_rows_ = 12; // number of rows in map
    static const int num_cols_ = 12; // number of columns in map
    static const int max_npcs_ = 5;  // max non-player characters
    static const int max_rooms_ = 5; // max number of rooms

    int player_position_[2];              // player position (row,col)
    int dungeon_exit_[2];                 // exit location of the dungeon
    int npc_positions_[max_npcs_][3];     // stores the (row,col) positions of NPCs present on map and if they have been found
    //why is it 3 rather than 2
    int room_positions_[max_rooms_][2];   // stores the (row,col) positions of rooms present on map
    char map_data_[num_rows_][num_cols_]; // stores the character that will be shown at a given (row,col)

    int npc_count_;  // stores number of misfortunes currently on map
    int room_count_; // stores number of sites currently on map 
    
public:
    Map();

    void resetMap();
    char getMapdata(int row, int col);
    // getters
        //Player
    int getPlayerRow();
    int getPlayerCol();
        //exit
    int getDungeonExitRow();
    int getDungeonExitCol();

    int getRoomCount();
    int getNPCCount(); 
        //(row,col)
    int getNumRows();
    int getNumCols();
    int getMaxNpcs(); //added
    int getMaxRooms(); //added
        //checking
    bool isOnMap(int row, int col);
    bool isNPCLocation(int row, int col);
    bool isRoomLocation(int row, int col);
    bool isExplored(int row, int col);
    bool isFreeSpace(int row, int col); 
    bool isDungeonExit(int row, int col);
 
    // setters
    void setPlayerPosition(int row, int col);
    void setDungeonExit(int row, int col); 

    // other
    void displayMap();
    bool move(char);
    //bool move(string);
    bool addNPC(int row, int col); //get it
    bool addRoom(int row, int col); //get it
    bool removeNPC(int row, int col); //after talking to NPC
    bool removeRoom(int row, int col); //after clearing room
    void exploreSpace(int row, int col);
};

#endif