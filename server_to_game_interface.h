#include <inttypes.h>

typedef struct command_struct
{
    int command_opcode;
    char* args;
    int args_len;
} Command;

typedef struct response_struct
{
    int ret_code;
    char* msg;
    int msg_len;
} Response;

