#ifndef BOARD_HPP
#define BOARD_HPP
#include <string>
#include <vector>
#include "BoardObj.h"

class Board{
    public:
        
        Board();
        void printBoard();
        bool suggestionMove(BoardObj&, BoardObj&, int);
        bool movePiece(BoardObj&, int);
        


    private:
        bool isRoom(int);
        bool hallwayClear(int);
        bool isAdjacent(int, int);
        bool isPassage(int, int);
        BoardObj getObj(BoardObj a, int loc);
        bool removeObj(BoardObj a, int loc);

        static const int num_locations = 21;
        static const int num_rooms = 9;
        static const int num_hallways = 12;
        int roomNumbers[9] = {0, 2, 4, 8, 10, 12, 16, 18, 20};
        
        std::vector<int> adjacentLocations[11];
        std::vector<BoardObj> main_board[num_locations];



};

#endif