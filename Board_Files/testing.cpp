#include <iostream>
#include "board.h"
#include "BoardObj.h"

int main(){
    //test interface
    std::string p1s = "MISS_SCARLETT";
    std::string p2s = "PROFESSOR_PLUM";
    std::string p3s = "COLONEL_MUSTARD";
    std::string p4s = "MRS_PEACOCK";
    std::string p5s = "MR_GREEN";
    std::string p6s = "MRS_WHITE";
    
    // BoardObj p1 = BoardObj("MISS_SCARLETT", 3, 1);
    // BoardObj p2 = BoardObj("PROFESSOR_PLUM", 5, 1);
    // BoardObj p3 = BoardObj("COLONEL_MUSTARD", 7, 1);
    // BoardObj p4 = BoardObj("MRS_PEACOCK", 13, 1);
    // BoardObj p5 = BoardObj("MR_GREEN", 17, 1);
    // BoardObj p6 = BoardObj("MRS_WHITE", 19, 1);
    Board a = Board();
    a.printBoard();
    a.movePiece(p1s, 4);
    std::cout << "after move piece" << std::endl;
    a.suggestionMove(p2s, p4s, 5);
    std::cout << "in between" << std::endl;
    a.suggestionMove(p2s, p4s, 4);
    std::cout << "moving Mrs White to 4" << std::endl;
    a.suggestionMove(p1s, p6s, 4);
    std::cout << "moving Mrs. White to 7" << std::endl;
    a.movePiece(p6s, 7);
    a.printBoard();
    std::cout << "attempt to move Miss Scarlett to 7 (fail)" << std::endl;
    a.movePiece(p1s, 7);
    std::cout << "Move mustard to 4" << std::endl;
    a.movePiece(p3s, 4);
    a.printBoard();
    a.movePiece(p6s, 16);
    std::cout << "move mrs. white to 16 via passage" << std::endl;
    a.printBoard();
    return 0;
}