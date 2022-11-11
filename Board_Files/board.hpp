#ifndef BOARD_HPP
#define BOARD_HPP
#include <string>
#include <vector>

class Board{
    public:
        Board();



    private:
        static const int num_locations = 21;
        vector<BoardCharacter> main_board[num_locations];



};

#endif