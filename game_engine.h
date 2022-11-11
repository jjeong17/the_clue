#include <vector>
#include <string>

class Game;
class Player_Manager;
class Player;
class Board;
class Location;
class Deck;
class Card;


class Game
{
private:
    static int num_games; // Counter to store the number of active games
    int id; // The unique id of this game
    Player_Manager* player_manager; // The player manager for this game instance
    Board* board; // The board for this game instance
    Deck* deck; // The deck for this game instance
public:
    Game();
    ~Game();
    int add_player(int);
    int start_game();
    int end_game();
    int make_move(int);
    int get_game_id();
};

class Player_Manager
{
private:
    int num_players;
    std::vector<Player*> player_list;
    std::string available_characters[6] = {"Colonel Mustard", "Mrs White", "Professor Plum", "Mrs. Peacock", "Mr. Green", "Ms. Scarlett"};
public:
    Player_Manager();
    ~Player_Manager();
    int add_player(int);
};

class Player
{
private:
    int player_id; // This will be assigned by the server
    int position; // Position at imaginary "table" -- to determine who goes in what order
    std::string selected_character;
    std::string username;
    std::vector<Card*> hand;
public:
    Player();
    Player(int, int);
    ~Player();
};

class Board
{
private:
    Location* locations[26];
public:
    Board();
    ~Board();
};

class Location
{
private:
    int index;
public:
    Location();
    ~Location();
};