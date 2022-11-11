#include "BoardCharacter.hpp"

BoardCharacter::BoardCharacter(){
    m_name = "None";
    m_board_location = 0;
}
BoardCharacter::BoardCharacter(std::string name, int location){
    m_name = name;
    m_board_lcoation = location;
}
std::string BoardCharacter::CharName(){
    return m_name;
}
int BoardCharacter::CharLocation(){
    return m_board_location;
}