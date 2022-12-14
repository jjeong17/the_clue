#ifndef BOARD_HPP
#define BOARD_HPP
#include <string>
#include <vector>
#include "BoardObj.h"

class Board{
    public:
        
        //sets up a regular game with default names
        Board();
        
        //sets up a game with custom names for characters
        Board(std::string, std::string, std::string, std::string,
        std::string, std::string);
        
        int* getPlayerLocations();

        //prints the current status of the board
        void printBoard();
        
        //p1 makes a suggestion of p2 in location given
        bool suggestionMove(std::string p1, std::string p2, int loc);
        
        //moves p1 to loc 
        bool movePiece(std::string p1, int loc);

        int bytes_length();
        void getBytes(int*);
        
    private:
        std::pair<int, int> findObject(std::string);
        bool isRoom(int);
        bool hallwayClear(int);
        bool isAdjacent(int, int);
        bool isPassage(int, int);
        BoardObj getObj(BoardObj a, int loc);
        bool removeObj(int, int);

        static const int num_locations = 21;
        static const int num_rooms = 9;
        static const int num_hallways = 12;
        int roomNumbers[9] = {0, 2, 4, 8, 10, 12, 16, 18, 20};
        
        std::vector<int> adjacentLocations[11];
        std::vector<BoardObj> main_board[num_locations];

};

#endif