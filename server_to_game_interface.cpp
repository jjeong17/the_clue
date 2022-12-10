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

extern "C" std::vector<Game*>* initialize_game_engine();
extern "C" Response* parse_command(std::vector<Game*>*, Command*);
extern "C" Command* alloc_command(int, char*, int);
extern "C" void free_response(Response*);

Response* parse_new_game_command(std::vector<Game*>*, Command*);
Response* parse_join_game_command(std::vector<Game*>*, Command*);
Response* parse_make_move_command(std::vector<Game*>*, Command*);
Response* parse_make_suggestion_command(std::vector<Game*>*, Command*);
Response* parse_periodic_checkin(std::vector<Game*>*, Command*);
Response* parse_make_accusation_command(std::vector<Game*>*, Command*);

// List of all the current games
// std::vector<Game*>* game_list;

std::vector<Game*>* initialize_game_engine()
{
    std::vector<Game*>* game_list = new std::vector<Game*>;
    return game_list;
}

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

Response* parse_command(std::vector<Game*>* game_list, Command* command_in)
{
    Response* response = NULL;

    if (game_list == NULL)
    {
        // 24: Game does not exist retcode
        response = alloc_response(24, "Game List is NULL\n", strlen("Game List is NULL\n") + 1);
        return response;
    }

    switch (command_in->command_opcode)
    {
        case 5:
            // periodic checkin command
            // response = alloc_response(6, "loud and clear", strlen("loud and clear") + 1);
            response = parse_periodic_checkin(game_list, command_in);
            free_command(command_in);
            return response;
        case 14:
            // New Game command
            response = parse_new_game_command(game_list, command_in);
            free_command(command_in);
            return response;
        case 15:
            // Join Game command
            response = parse_join_game_command(game_list, command_in);
            free_command(command_in);
            return response;
        case 31:
            // Make Move command
            response = parse_make_move_command(game_list, command_in);
            free_command(command_in);
            return response;
        case 36:
            // Make suggestion command
            response = parse_make_suggestion_command(game_list, command_in);
            free_command(command_in);
            return response;
        case 39:
            // Make accusation command
            response = parse_make_accusation_command(game_list, command_in);
            free_command(command_in);
            return response;
        default:
            // 99: Unknown command retcode
            response = alloc_response(99, NULL, 0);
            free_command(command_in);
            return response;
    }
}

Response* parse_new_game_command(std::vector<Game*>* game_list, Command* command_in)
{
    int new_game_id;
    int player_id;
    Response* response;

    Game* new_game = new Game();
    new_game_id = new_game->get_game_id();

    // Add player to the game
    // player id is args+0x0 interpreted as a 4-byte integer
    player_id = *((int*)(command_in->args));
    new_game->add_player(player_id);

    game_list->push_back(new_game);

    // 21: Game succesfully created ret code
    // msg: 4-byte game id cast to a char*
    response = alloc_response(21, (char*)(&new_game_id), 4);

    return response;
}

Response* parse_join_game_command(std::vector<Game*>* game_list, Command* command_in)
{
    Response* response;
    int game_index = -1;
    int requested_game_id;
    int add_ret_code;
    int player_id;

    
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
    response = alloc_response(23, (char*)(&requested_game_id), 4);
    return response;
}

Response* parse_periodic_checkin(std::vector<Game*>* game_list, Command* command_in)
{
    Response* response;
    int game_id;
    int player_id;
    int game_index = -1;

    int* character_locations;

    char loc_string[6] = {'\0'};    

    // game id is args+0x0 interpreted as a 4-byte integer
    game_id = *((int*)command_in->args);

    // player_id is args+0x4 interpreted as a 4-byte integer
    player_id = *((int*)(command_in->args + 4));

    if (game_id == -1) // -1 Means the player has not yet joined a game
    {
        // 8: periodic checkin, don't update board 
        response = alloc_response(8, "filler", strlen("filler") + 1);
        return response;
    }
    for (int i = 0; i < game_list->size(); i++)
    {
        if ((*game_list)[i]->get_game_id() == game_id)
        {
            game_index = i;
        }
    }
    if (game_index == -1)
    {
        // 7: Problem with periodic checkin retcode
        response = alloc_response(7, "Periodic Checkin Problem: Game dne\n", strlen("Periodic Checkin Problem: Game dne\n") + 1);
        return response;
    }
    character_locations = (*game_list)[game_index]->get_player_locations();
    for (int i = 0; i < 6; i++)
    {
        loc_string[i] = (character_locations[i] + 1) & 0xFF;
    }

    response = alloc_response(6, loc_string, strlen(loc_string) + 1);
    return response;
}

Response* parse_make_move_command(std::vector<Game*>* game_list, Command* command_in)
{
    Response* response;
    int game_id;
    int player_id;
    int move_option;
    int game_index = -1;
    int ret_code;

    // game id is args+0x0 interpreted as a 4-byte integer
    game_id = *((int*)command_in->args);

    // player_id is args+0x4 interpreted as a 4-byte integer
    player_id = *((int*)(command_in->args + 4));

    // move option is args+0x8 interpreted as a 4-byte integer
    move_option = *((int*)(command_in->args + 8));

    // printf("Game id: %d, move option: %d\n", game_id, move_option);

    for (int i = 0; i < game_list->size(); i++)
    {
        if ((*game_list)[i]->get_game_id() == game_id)
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
    ret_code = (*game_list)[game_index]->make_move(player_id, move_option);

    if (ret_code == 0)
    {
        response = alloc_response(25, "Invalid Move\n", strlen("Invalid Move\n") + 1);
        return response;
    }

    response = alloc_response(25, "Move made succesfully\n", strlen("Move made succesfully\n") + 1);
    return response;
}

Response* parse_make_accusation_command(std::vector<Game*>* game_list, Command* command_in)
{
    Response* response;
    int game_id;
    int player_id;
    int target_player_id;
    int weapon_id;
    int target_location;
    int game_index;

    bool right_or_not;

    // game id is args+0x0 interpreted as a 4-byte integer
    game_id = *((int*)command_in->args);

    // player_id is args+0x4 interpreted as a 4-byte integer
    player_id = *((int*)(command_in->args + 4));

    // target_player_id is args+0x8 interpreted as a 4-byte integer
    target_player_id = *((int*)(command_in->args + 8));
    
    // weapon_id is args+0xc interpreted as a 4-byte integer
    weapon_id = *((int*)(command_in->args + 0xc));

    // target_location is args+0x10 interpreted as a 4-byte integer
    target_location = *((int*)(command_in->args + 0x10));

    for (int i = 0; i < game_list->size(); i++)
    {
        if ((*game_list)[i]->get_game_id() == game_id)
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

    // printf("%d %d %d %d %d\n", game_id, player_id, target_player_id, weapon_id, target_location);
    right_or_not = (*game_list)[game_index]->make_accusation(player_id, target_player_id, weapon_id, target_location);

    if (right_or_not)
    {
        response = alloc_response(70, "Accusation was correct!\n", strlen("Accusation was correct!\n") + 1);
        return response;
    }
    else
    {
        response = alloc_response(71, "Accusation was not correct!\n", strlen("Accusation was not correct!\n") + 1);
        return response;
    }


}

Response* parse_make_suggestion_command(std::vector<Game*>* game_list, Command* command_in)
{
    Response* response;
    int game_id;
    int player_id;
    int target_player_id;
    int weapon_id;
    int target_location;
    int game_index;

    // game id is args+0x0 interpreted as a 4-byte integer
    game_id = *((int*)command_in->args);

    // player_id is args+0x4 interpreted as a 4-byte integer
    player_id = *((int*)(command_in->args + 4));

    // target_player_id is args+0x8 interpreted as a 4-byte integer
    target_player_id = *((int*)(command_in->args + 8));
    
    // weapon_id is args+0xc interpreted as a 4-byte integer
    weapon_id = *((int*)(command_in->args + 0xc));

    // target_location is args+0x10 interpreted as a 4-byte integer
    target_location = *((int*)(command_in->args + 0x10));

    for (int i = 0; i < game_list->size(); i++)
    {
        if ((*game_list)[i]->get_game_id() == game_id)
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

    // printf("%d %d %d %d %d\n", game_id, player_id, target_player_id, weapon_id, target_location);
    (*game_list)[game_index]->make_suggestion(player_id, target_player_id, weapon_id, target_location);

    response = alloc_response(25, "Suggestion made succesfully\n", strlen("Suggestion made succesfully\n") + 1);
    return response;
}