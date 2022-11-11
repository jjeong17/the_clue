#ifndef BoardObj_HPP
#define BoardObj_HPP
#include <iostream>
#include <string>

class BoardObj{
    public:
        BoardObj();
        BoardObj(std::string, int, int);
        BoardObj& operator=(const BoardObj& rtSide);
        std::string getName();
        int getLocation();
        void setLocation(int);
        int getType();
        
    private:
        std::string m_name;
        int m_board_location;
        int m_obj_type; //0 means weapon, 1 means player
    
};

#endif