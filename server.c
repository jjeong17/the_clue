#include "server_to_game_interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

/*
* Compile with:
* gcc server2.c -o server2 -lpthread
*/

#define PORT "8687"
#define BACKLOG 10

#define MAX_CLIENTS 64
#define BUFFER_SZ 2048

void* initialize_game_engine();
Response* parse_command(void*, Command*);
Command* alloc_command(int, char*, int);
void free_response(Response*);

int parse_msg(void*, char**, char**, int);


static _Atomic unsigned int cli_count = 0;
static int uid = 10;

/* Client structure */
typedef struct
{
	struct sockaddr_storage address;
	int sockfd;
	int uid;
} client_t;

typedef struct
{
    client_t* cli;
    void* game_engine_ptr;
} thread_args;

client_t *clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void str_overwrite_stdout() 
{
    printf("\r%s", "> ");
    fflush(stdout);
}

void str_trim_lf (char* arr, int length) 
{
  int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
      arr[i] = '\0';
      break;
    }
  }
}

/* Add clients to queue */
void queue_add(client_t *cl)
{
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i)
    {
		if(!clients[i])
        {
			clients[i] = cl;
			break;
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

void queue_remove(int uid)
{
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid == uid){
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

void send_message(char *s, int uid)
{
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i<MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid != uid){
				if(write(clients[i]->sockfd, s, strlen(s)) < 0){
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

int unpack_int(char* byte_array, int offset)
{
    int ret_int;
    memcpy(&ret_int, byte_array + offset, sizeof(int));

    return ntohl(ret_int);
}

int pack_int(char* byte_array, int offset, int to_pack)
{
    int n_to_pack = htonl(to_pack);
    memcpy(byte_array + offset, &n_to_pack, sizeof(int));

    return sizeof(int);
}

/* Handle all communication with the client */
void *handle_client(void* arg)
{
    char* recv_buf;
    char head_buf[12] = {0};
    int magic_bytes;
    int msg_len;
    int received;
    int bytes_read = 0;

    char* send_buf = calloc(1000, sizeof(char));
    int send_magic_bytes = 0x1234;
    int send_msg_len;
    int bytes_sent = 0;
    int bytes_packed = 0;

    int msg_type;

    thread_args* thread_arguments = (thread_args *)arg;
	
	cli_count++;
    client_t *cli = thread_arguments->cli;

    void* game_engine_ptr = thread_arguments->game_engine_ptr;

    // Name
    while (1)
    {
        bytes_read = 0;
        bytes_packed = 0;
        msg_len = 0;
        send_msg_len = 0;

        received = recv(cli->sockfd, head_buf, 12, 0);

        if (received == 0)
        {
            printf("Someone has left\n");
            break;
        }
        if (received == -1)
        {
            printf("ERROR: someone has left unexpectedly\n");
            break;
        }

        magic_bytes = unpack_int(head_buf, bytes_read);
        bytes_read += 4;
        cli->uid = unpack_int(head_buf, bytes_read);
        bytes_read += 4;
        msg_len = unpack_int(head_buf, bytes_read);
        bytes_read += 4;

        printf("Incomming message: \n");
        printf("Client ID: 0x%x\nMsg size: 0x%x\n", cli->uid, msg_len);

        memset(head_buf, 0, 12);
        if (msg_len < 4 || msg_len > 1000)
        {
            send_msg_len = strlen("Bad message!\n") + 1;
            bytes_packed += pack_int(send_buf, bytes_packed, send_magic_bytes);
            bytes_packed += pack_int(send_buf, bytes_packed, send_msg_len);
            memcpy(send_buf + bytes_packed, "Bad message!\n", send_msg_len);
            printf("Sending Response!\n");
            if (send(cli->sockfd, send_buf, bytes_packed + send_msg_len, 0) == -1)
            {
                perror("send");
            }
            continue;
        }
        recv_buf = calloc(msg_len, sizeof(char));
        recv(cli->sockfd, recv_buf, msg_len, 0);

        send_buf = send_buf + 8;
        send_msg_len = parse_msg(game_engine_ptr, &recv_buf, &send_buf, msg_len);
        send_buf = send_buf - 8;
        bytes_packed += pack_int(send_buf, bytes_packed, send_magic_bytes);
        bytes_packed += pack_int(send_buf, bytes_packed, send_msg_len);
        printf("Sending Response!\n");
        if (send(cli->sockfd, send_buf, bytes_packed + send_msg_len, 0) == -1)
        {
            perror("send");
        }
        if (recv_buf != NULL)
        {
            free(recv_buf);
        }
        memset(send_buf, 0, 1000);
    }
    /* Delete client from queue and yield thread */
	close(cli->sockfd);
    queue_remove(cli->uid);
    free(cli);
    cli_count--;
    free(thread_arguments);
    pthread_detach(pthread_self());

	return NULL;
}

int parse_msg(void* game_engine_ptr, char** recv_buf, char** send_buf, int recv_buf_len)
{
    int bytes_read = 0;
    int bytes_packed = 0;
    int command_op;

    int send_msg_len;

    char* args;
    int args_len = recv_buf_len - 4;

    Command* command;
    Response* response;
    
    command_op = unpack_int(*recv_buf, bytes_read);
    bytes_read += 4;

    if (command_op == 0)
    {
        // Client checkin message retcode: 10
        bytes_packed += pack_int(*send_buf, bytes_packed, 10);
        send_msg_len = bytes_packed;
        return send_msg_len;
    }

    args = calloc(args_len, sizeof(char));
    memcpy(args, (*recv_buf) + bytes_read, args_len);
    command = alloc_command(command_op, args, args_len);
    if (command == NULL)
    {
        printf("Command Allocation Error\n");
        return 0;
    }
    response = parse_command(game_engine_ptr, command);
    if (response == NULL)
    {
        printf("Command parse error\n");
        return 0;
    }
    send_msg_len = response->msg_len + sizeof(int)*2;
    bytes_packed += pack_int(*send_buf, bytes_packed, response->ret_code);
    bytes_packed += pack_int(*send_buf, bytes_packed, response->msg_len);
    memcpy((*send_buf) + bytes_packed, response->msg, response->msg_len);
    free_response(response);
    return send_msg_len;
}


void* get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    printf("Only IPv4 supported\n");
}

int main()
{
    int sock_fd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information

    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET_ADDRSTRLEN];
    int ret_code;

    pthread_t tid;

    void* game_engine_ptr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    ret_code = getaddrinfo(NULL, PORT, &hints, &servinfo);

    if (ret_code != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret_code));
        return 1;
    }

    //loop through the linked list of results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock_fd == -1)
        {
            perror("server: socket");
            continue;
        }
        if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }
        if (bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sock_fd);
            perror("server: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }
    if (listen(sock_fd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    game_engine_ptr = initialize_game_engine();
    printf("server: waiting for connections...\n");

    // main accept() loop
	while(1)
    {
		sin_size = sizeof(their_addr);
		new_fd = accept(sock_fd, (struct sockaddr*)&their_addr, &sin_size);

		/* Check if max clients is reached */
		if((cli_count + 1) == MAX_CLIENTS){
			printf("Max clients reached. Rejected: ");
			inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
            printf("%s\n", s);
			close(new_fd);
			continue;
		}

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
        printf("server: got connection from %s\n", s);

		/* Client settings */
		client_t* cli = (client_t *)calloc(1, sizeof(client_t));
		cli->address = their_addr;
		cli->sockfd = new_fd;
		cli->uid = uid++;

        thread_args* thread_arguments = (thread_args *)calloc(1, sizeof(thread_args)); 
        thread_arguments->cli = cli;
        thread_arguments->game_engine_ptr = game_engine_ptr;

		/* Add client to the queue and fork thread */
		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void*)thread_arguments);

		/* Reduce CPU usage */
		// sleep(1);
	}

	return EXIT_SUCCESS;
}