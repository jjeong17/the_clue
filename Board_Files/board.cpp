#include "board.hpp"
#include "BoardObj.hpp"
#include <iostream>

Board::Board(){
    BoardObj p1 = BoardObj("Miss Scarlett", 3, 1);
    BoardObj p2 = BoardObj("Professor Plum", 5, 1);
    BoardObj p3 = BoardObj("Col Mustard", 7, 1);
    BoardObj p4 = BoardObj("Mrs. Peacock", 13, 1);
    BoardObj p5 = BoardObj("Mr. Green", 17, 1);
    BoardObj p6 = BoardObj("Mrs. White", 19, 1);

    main_board[p1.getLocation()].push_back(p1);
    main_board[p2.getLocation()].push_back(p2);
    main_board[p3.getLocation()].push_back(p3);
    main_board[p4.getLocation()].push_back(p4);
    main_board[p5.getLocation()].push_back(p5);
    main_board[p6.getLocation()].push_back(p6);

}



void Board::print(){
    std::cout << "board print" << std::endl;
}