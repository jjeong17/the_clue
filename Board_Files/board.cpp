#include "board.hpp"
#include "BoardCharacter.hpp"
#include <iostream>

Board::Board(){
    BoardCharacter("Miss Scarlett", 3);
    BoardCharacter("Professor Plum", 5);
    BoardCharacter("Col Mustard", 7);
    BoardCharacter("Mrs. Peacock", 13);
    BoardCharacter("Mr. Green", 17);
    BoardCharacter("Mrs. White", 19);
}

void Board::print(){
    std::cout << "board print" << std::endl;
}