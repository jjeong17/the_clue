#include <iostream>
#include "board.h"
#include "BoardObj.h"

int main(){
    //test interface
    BoardObj p1 = BoardObj("Miss Scarlett", 3, 1);
    BoardObj p2 = BoardObj("Professor Plum", 5, 1);
    BoardObj p3 = BoardObj("Col Mustard", 7, 1);
    BoardObj p4 = BoardObj("Mrs. Peacock", 13, 1);
    BoardObj p5 = BoardObj("Mr. Green", 17, 1);
    BoardObj p6 = BoardObj("Mrs. White", 19, 1);
    Board a = Board();
    a.printBoard();
    a.movePiece(p1, 4);
    a.suggestionMove(p2, p4, 5);
    a.suggestionMove(p2, p4, 4);
    std::cout << "moving Mrs White to 4" << std::endl;
    a.suggestionMove(p1, p6, 4);
    std::cout << "moving Mrs. White to 7" << std::endl;
    a.movePiece(p6, 7);
    a.printBoard();
    std::cout << "attempt to move Miss Scarlett to 7 (fail)" << std::endl;
    a.movePiece(p1, 7);
    std::cout << "Move mustard to 4" << std::endl;
    a.movePiece(p3, 4);
    a.printBoard();
    a.movePiece(p6, 16);
    std::cout << "move mrs. white to 16 via passage" << std::endl;
    a.printBoard();
    return 0;
}