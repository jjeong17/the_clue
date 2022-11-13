#include "BoardObj.h"

BoardObj::BoardObj(){
    m_name = "None";
    m_board_location = 0;
    m_obj_type = 0;

}
BoardObj::BoardObj(std::string name, int location, int type){
    m_name = name;
    m_board_location = location;
    m_obj_type = type;
}
BoardObj& BoardObj::operator=(const BoardObj& rtSide){
    m_name = rtSide.m_name;
    m_board_location = rtSide.m_board_location;
    m_obj_type = rtSide.m_obj_type;
    return *this;
}

std::string BoardObj::getName(){
    return m_name;
}
int BoardObj::getLocation(){
    return m_board_location;
}
void BoardObj::setLocation(int new_loc){
    m_board_location = new_loc;
}
int BoardObj::getType(){
    return m_obj_type;
}