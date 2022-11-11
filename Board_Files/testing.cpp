#include <iostream>
#include "board.hpp"
#include "BoardObj.hpp"

int main(){
    //test interface
    BoardObj scarlett = BoardObj("Miss Scarlett",  3, 1);
    Board a = Board();
    a.printBoard();
    a.movePiece(scarlett, 4);
    a.printBoard();
    return 0;
}