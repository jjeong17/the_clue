#include "board.h"
#include "BoardObj.h"
#include <iostream>
#include <vector>

Board::Board(){
    BoardObj p1 = BoardObj("MISS_SCARLETT", 3, 1);
    BoardObj p2 = BoardObj("PROFESSOR_PLUM", 5, 1);
    BoardObj p3 = BoardObj("COLONEL_MUSTARD", 7, 1);
    BoardObj p4 = BoardObj("MRS_PEACOCK", 13, 1);
    BoardObj p5 = BoardObj("MR_GREEN", 17, 1);
    BoardObj p6 = BoardObj("MRS_WHITE", 19, 1);

    main_board[p1.getLocation()].push_back(p1);
    main_board[p2.getLocation()].push_back(p2);
    main_board[p3.getLocation()].push_back(p3);
    main_board[p4.getLocation()].push_back(p4);
    main_board[p5.getLocation()].push_back(p5);
    main_board[p6.getLocation()].push_back(p6);

    adjacentLocations[0].push_back(1);
    adjacentLocations[0].push_back(5);

    adjacentLocations[1].push_back(1);
    adjacentLocations[1].push_back(3);
    adjacentLocations[1].push_back(6);
    
    adjacentLocations[2].push_back(3);
    adjacentLocations[2].push_back(7);

    adjacentLocations[4].push_back(5);
    adjacentLocations[4].push_back(9);
    adjacentLocations[4].push_back(13);

    adjacentLocations[5].push_back(6);
    adjacentLocations[5].push_back(9);
    adjacentLocations[5].push_back(11);
    adjacentLocations[5].push_back(14);

    adjacentLocations[6].push_back(7);
    adjacentLocations[6].push_back(11);
    adjacentLocations[6].push_back(15);

    adjacentLocations[8].push_back(13);
    adjacentLocations[8].push_back(17);

    adjacentLocations[9].push_back(14);
    adjacentLocations[9].push_back(17);
    adjacentLocations[9].push_back(19);

    adjacentLocations[10].push_back(15);
    adjacentLocations[10].push_back(19);

}

Board::Board(std::string obj1, std::string obj2, 
std::string obj3, std::string obj4, 
std::string obj5, std::string obj6){
    BoardObj p1 = BoardObj(obj1, 3, 1);
    BoardObj p2 = BoardObj(obj2, 5, 1);
    BoardObj p3 = BoardObj(obj3, 7, 1);
    BoardObj p4 = BoardObj(obj4, 13, 1);
    BoardObj p5 = BoardObj(obj5, 17, 1);
    BoardObj p6 = BoardObj(obj6, 19, 1);

    main_board[p1.getLocation()].push_back(p1);
    main_board[p2.getLocation()].push_back(p2);
    main_board[p3.getLocation()].push_back(p3);
    main_board[p4.getLocation()].push_back(p4);
    main_board[p5.getLocation()].push_back(p5);
    main_board[p6.getLocation()].push_back(p6);

    adjacentLocations[0].push_back(1);
    adjacentLocations[0].push_back(5);

    adjacentLocations[1].push_back(1);
    adjacentLocations[1].push_back(3);
    adjacentLocations[1].push_back(6);
    
    adjacentLocations[2].push_back(3);
    adjacentLocations[2].push_back(7);

    adjacentLocations[4].push_back(5);
    adjacentLocations[4].push_back(9);
    adjacentLocations[4].push_back(13);

    adjacentLocations[5].push_back(6);
    adjacentLocations[5].push_back(9);
    adjacentLocations[5].push_back(11);
    adjacentLocations[5].push_back(14);

    adjacentLocations[6].push_back(7);
    adjacentLocations[6].push_back(11);
    adjacentLocations[6].push_back(15);

    adjacentLocations[8].push_back(13);
    adjacentLocations[8].push_back(17);

    adjacentLocations[9].push_back(14);
    adjacentLocations[9].push_back(17);
    adjacentLocations[9].push_back(19);

    adjacentLocations[10].push_back(15);
    adjacentLocations[10].push_back(19);

}

int* Board::getPlayerLocations()
{
    int* character_locations = (int*)calloc(6, sizeof(int));

    for(int i = 0; i < num_locations; ++i){
        for(int j = 0; j < main_board[i].size(); ++j){
            if (main_board[i][j].getName() != "None")
            {
                if (main_board[i][j].getName() == "MISS_SCARLETT")
                {
                    character_locations[0] = i;
                }
                else if (main_board[i][j].getName() == "PROFESSOR_PLUM")
                {
                    character_locations[1] = i;
                }
                else if(main_board[i][j].getName() == "COLONEL_MUSTARD")
                {
                    character_locations[2] = i;
                }
                else if(main_board[i][j].getName() == "MRS_PEACOCK")
                {
                    character_locations[3] = i;
                }
                else if(main_board[i][j].getName() == "MR_GREEN")
                {
                    character_locations[4] = i;
                }
                else if(main_board[i][j].getName() == "MRS_WHITE")
                {
                    character_locations[5] = i;
                }
                else
                {
                    std::cout << "This should not be happening" << std::endl;
                }
            }
        }
    }
    return character_locations;
}

void Board::printBoard(){
    std::cout << "Printing Board" << std::endl;
    for(int i = 0; i < num_locations; ++i){
        std::cout << "row: " << i << std::endl;
        for(int j = 0; j < main_board[i].size(); ++j){
            std::cout << "\t" << main_board[i][j].getName() << std::endl;

        }
    }
}
std::pair<int, int> Board::findObject(std::string my_str){
    for(int i = 0; i < num_locations; ++i){
        for(int j = 0; j < main_board[i].size(); ++j){
            if(main_board[i][j].getName() == my_str){
                std::pair<int, int> my_pair;
                my_pair.first = i;
                my_pair.second = j;
                return my_pair;
            }
        }
    }
    std::cout << "reached end couldn't find object" << std::endl;
    throw "Error";
}


bool Board::isRoom(int a){
    for(int i = 0; i < num_rooms; i++){
        if(a == roomNumbers[i]){
            return true;
        }
    }
    return false;
}

bool Board::hallwayClear(int a){
    for(int i = 0; i < main_board[a].size(); ++i){
        if(main_board[a][i].getType() == 1){
            return false;
        }
    }
    return true;
}

bool Board::isAdjacent(int a, int b){
    if(isRoom(a)){
        for(int i = 0; i < adjacentLocations[a/2].size(); i++){
            if(adjacentLocations[a/2][i] == b){
                return true;
            }
        }
    }else if(isRoom(b)){
        for(int j = 0; j < adjacentLocations[b/2].size(); j++){
            if(adjacentLocations[b/2][j] == a){
                return true;
            }
        }
    }
    return false;
}

bool Board::isPassage(int a, int b){
    bool isPassage = false;
    if(a == 0 && b == 20){
        isPassage = true;
    }else if(a == 20 && b == 0){
        isPassage = true;
    }else if(a == 4 && b == 16){
        isPassage = true;
    }else if(a == 16 && b == 4){
        isPassage = true;
    }
    return isPassage;
}

bool Board::suggestionMove(std::string p1,
std::string p2, int location){

    std::pair<int, int> coordinate_pair1;
    std::pair<int, int> coordinate_pair2;
    coordinate_pair1 = findObject(p1);
    BoardObj p1Obj = main_board[coordinate_pair1.first][coordinate_pair1.second];
    coordinate_pair2 = findObject(p2);
    BoardObj p2Obj = main_board[coordinate_pair2.first][coordinate_pair2.second];
    if(!isRoom(location)){
        std::cout << "is not a room" << std::endl;
        return false;
    }
    std::pair<int, int> my_pair = findObject(p1);
    if(my_pair.first != location){
        std::cout << "player not in location" << std::endl;
        return false;
    }
    
    removeObj(coordinate_pair2.first, coordinate_pair2.second);
    p2Obj.setLocation(location);
    main_board[location].push_back(p2Obj);
    return true;
}

BoardObj Board::getObj(BoardObj a, int loc){
    
    for(int i = 0; i < main_board[loc].size(); i++){
        if(a.getName() == main_board[loc][i].getName()){
            return main_board[loc][i];
        }
    }
    throw "Error Object Not Found";
}

bool Board::removeObj(int a, int b){
    main_board[a].erase(main_board[a].begin() + b);
    return true;
}

bool Board::movePiece(std::string name, int location){
    std::pair<int, int> coords;
    coords = findObject(name);
    BoardObj pObj = main_board[coords.first][coords.second];

    int curr_index = 0;
    if(isRoom(location)){
        if(isAdjacent(coords.first, location) || 
        isPassage(coords.first, location)){
            removeObj(coords.first, coords.second);
            pObj.setLocation(location);
            main_board[location].push_back(pObj);
            return true;
        }
    }else{
        if(hallwayClear(location) && 
        isAdjacent(coords.first, location)){
            removeObj(coords.first, coords.second);
            pObj.setLocation(location);
            main_board[location].push_back(pObj);
            return true;
        }
    }
    return false;
}

int Board::bytes_length(){
    return 6;
}

void Board::getBytes(int* buffer){
    std::string names_array[6] = {"MISS_SCARLETT", "PROFESSOR_PLUM",
    "COLONEL_MUSTARD", "MRS_PEACOCK", "MR_GREEN", "MRS_WHITE"};
    std::pair<int, int> board_data;
    for(int i = 0; i < 6; i++){
        board_data = findObject(names_array[i]);
        buffer[i] = board_data.first;
    }
}