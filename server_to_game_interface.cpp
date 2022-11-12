/* 
* This will implement all of the mechanisms to allow the server
* to communicate with the game engine
* I will implement all of this -Scott
*/

#include "server_to_game_interface.h"
#include "game_engine.h"

#include <vector>
#include <iostream>

#include <string.h>
#include <stdlib.h>

extern "C" Response* parse_command(Command*);
extern "C" Command* alloc_command(int, char*, int);
extern "C" void free_response(Response*);

Response* parse_new_game_command(Command*);
Response* parse_join_game_command(Command*);

// List of all the current games
std::vector<Game*>* game_list;


Response* alloc_response(int ret_code, char* msg, int msg_len)
{
    Response* new_response = (Response*)calloc(1, sizeof(Response));
    new_response->ret_code = ret_code;
    new_response->msg_len = msg_len;
    new_response->msg = (char*)calloc(msg_len, sizeof(char));
    memcpy(new_response->msg, msg, msg_len);
    return new_response;
}

void free_response(Response* response)
{
    if (response != NULL)
    {
        free(response->msg);
        free(response);
    }
}

Command* alloc_command(int command_opcode, char* args, int args_len)
{
    Command* new_command = (Command*)calloc(1, sizeof(Command));
    new_command->command_opcode = command_opcode;
    new_command->args_len = args_len;
    new_command->args = (char*)calloc(args_len, sizeof(char));
    memcpy(new_command->args, args, args_len);
    return new_command;
}

void free_command(Command* command)
{
    if (command != NULL)
    {
        free(command->args);
        free(command);
    }
}

Response* parse_command(Command* command_in)
{
    Response* response = NULL; 
    switch (command_in->command_opcode)
    {
        case 14:
            // New Game command
            response = parse_new_game_command(command_in);
            free_command(command_in);
            return response;
        case 15:
            // Join Game command
            response = parse_join_game_command(command_in);
            free_command(command_in);
            return response;
        default:
            // 99: Unknown command retcode
            response = alloc_response(99, NULL, 0);
            free_command(command_in);
            return response;
    }
}

Response* parse_new_game_command(Command* command_in)
{
    int new_game_id;
    Response* response;

    if (game_list == NULL)
    {
        game_list = new std::vector<Game*>;
    }
    Game* new_game = new Game();
    new_game_id = new_game->get_game_id();
    game_list->push_back(new_game);

    // 21: Game succesfully created ret code
    // msg: 4-byte game id cast to a char*
    response = alloc_response(21, (char*)new_game_id, 4);

    return response;
}

Response* parse_join_game_command(Command* command_in)
{
    Response* response;
    int game_index = -1;
    int requested_game_id;
    int add_ret_code;
    int player_id;

    if (game_list == NULL)
    {
        // 24: Game does not exist retcode
        response = alloc_response(24, "Requested Game Does Not Exist\n", strlen("Requested Game Does Not Exist\n") + 1);
        return response;
    }
    
    // game id is args+0x0 interpreted as a 4-byte integer
    requested_game_id = *((int*)command_in->args);

    for (int i = 0; i < game_list->size(); i++)
    {
        if ((*game_list)[i]->get_game_id() == requested_game_id)
        {
            game_index = i;
        }
    }
    if (game_index == -1)
    {
        // 24: Game does not exist retcode
        response = alloc_response(24, "Requested Game Does Not Exist\n", strlen("Requested Game Does Not Exist\n") + 1);
        return response;
    }

    // player id is args+0x4 interpreted as a 4-byte integer
    player_id = *((int*)(command_in->args+4));

    add_ret_code = (*game_list)[game_index]->add_player(player_id);

    if (add_ret_code == 0)
    {
        // 25: Requested game is full retcode
        response = alloc_response(25, "Requested game is full\n", strlen("Requested game is full\n") + 1);
        return response;
    }

    // Success case: 23: join success retcode
    response = alloc_response(23, NULL, 0);
    return response;
}

int sample_function(int i)
{
    return 0;
}