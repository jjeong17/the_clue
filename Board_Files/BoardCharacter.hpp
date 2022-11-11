#ifndef BOARDCHARACTER_HPP
#define BOARDCHARACTER_HPP
#include <iostream>
#include <string>



class BoardCharacter{
    public:
        BoardCharacter();
        std::string CharName();
        int CharLocation();

    private:
        std::string m_name;
        int m_board_location;
    
};

#endif