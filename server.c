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

/* Compile with:
    *
    * gcc server.c -o server
    * 
*/

#define PORT "8687"
#define BACKLOG 10 // how many pending connections

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void* get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    printf("Only IPv4 supported\n");
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

int main()
{
    /* This section is just for the demo */
    /* Get rid of it asap */

    int active_games[100] = {0};
    int active_game_index = 1;

    /* *** */



    int sock_fd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information

    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET_ADDRSTRLEN];
    int ret_code;

    // Temporary: should be dyn on heap
    char recv_buf[1024] = {0};
    int magic_bytes;
    int msg_len;
    int bytes_read = 0;
    int client_id;

    char send_buf[1024] = {0};
    int send_magic_bytes = 0x1234;
    int send_msg_len;
    int bytes_sent = 0;
    int bytes_packed = 0;

    int msg_type;

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

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    // main accept() loop
    while(1)
    {
        sin_size = sizeof(their_addr);
        new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
        printf("server: got connection from %s\n", s);

        if (!fork())
        {
            // this is the child process
            close(sock_fd); // child doesn't need the listener

            while(1){
                bytes_read = 0;
                msg_len = 0;
                send_msg_len = 0;
                bytes_packed = 0;

                recv(new_fd, recv_buf, 12, 0);

                magic_bytes = unpack_int(recv_buf, bytes_read);
                bytes_read += 4;
                client_id = unpack_int(recv_buf, bytes_read);
                bytes_read += 4;
                msg_len = unpack_int(recv_buf, bytes_read);
                bytes_read += 4;

                printf("Incomming message: \n");
                printf("Client ID: 0x%x\nMsg size: 0x%x\n", client_id, msg_len);

                memset(recv_buf, 0, 12);
                if (msg_len < 1000)
                {
                    recv(new_fd, recv_buf, msg_len, 0);
                }
                
                bytes_read = 0;
                msg_type = unpack_int(recv_buf, bytes_read);
                bytes_read += 4;

                if (msg_type == 0)
                {
                    send_msg_len = strlen("Hello client!") + 1;
                    bytes_packed += pack_int(send_buf, bytes_packed, send_magic_bytes);
                    bytes_packed += pack_int(send_buf, bytes_packed, send_msg_len);
                    memcpy(send_buf + bytes_packed, "Hello client!", send_msg_len);
                    printf("\nSending Response!\n");
                    if (send(new_fd, send_buf, bytes_packed + send_msg_len, 0) == -1)
                    {
                        perror("send");
                    }
                }
                else if (msg_type == 2)
                {
                    msg_type = unpack_int(recv_buf, bytes_read);
                    bytes_read += 4;
                    printf("Join request for game: %d received\n", msg_type);

                    send_msg_len = strlen("You have joined a game!") + 1;
                    bytes_packed += pack_int(send_buf, bytes_packed, send_magic_bytes);
                    bytes_packed += pack_int(send_buf, bytes_packed, send_msg_len);
                    memcpy(send_buf + bytes_packed, "You have joined a game!", send_msg_len);
                    printf("\nSending Response!\n");
                    if (send(new_fd, send_buf, bytes_packed + send_msg_len, 0) == -1)
                    {
                        perror("send");
                    }
                }
                else if (msg_type == 3)
                {
                    printf("Create request received\n");
                    printf("Game created: %d", active_game_index);
                    active_game_index += 1;

                    send_msg_len = strlen("Game created with id: 1") + 1;
                    bytes_packed += pack_int(send_buf, bytes_packed, send_magic_bytes);
                    bytes_packed += pack_int(send_buf, bytes_packed, send_msg_len);
                    memcpy(send_buf + bytes_packed, "Game created with id: 1", send_msg_len);
                    printf("\nSending Response!\n");
                    if (send(new_fd, send_buf, bytes_packed + send_msg_len, 0) == -1)
                    {
                        perror("send");
                    }
                }
            }
            close(new_fd);
            exit(0);
        }
        close(new_fd); // parent doesn't need this
    }

    return 0;
}