#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>


int main() 
{

    int master_socket;  
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
    int flags;
    if( (flags = fcntl(master_socket, F_GETFL)) == -1)
    {
        perror("flags not retrieved on TCP listening socket");
        exit(1);
    }


    if ( fcntl(master_socket, F_SETFL, flags | O_NONBLOCK) == -1)
    { 
	perror("Could not set socket in non-blocking mode");
	exit(1); 
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(master_socket, (struct sockaddr *)&addr, sizeof(addr))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }  
    if (listen(master_socket, 3) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }
    while(1) 
    {
        int client_socket_fd = accept(master_socket, NULL, NULL);
        if (client_socket_fd == -1) 
	{
            if (errno == EWOULDBLOCK)
            {
                printf("There is no pending connection, sleep for 2 seconds\n");
                sleep(2);
            } 
            else
            {
                perror("error when accepting connection");
                exit(1);
            }
        }
        else
        {
            char msg[] = "Hey Hi!!!\n";
            printf("Got a connection; writing message\n");
      	    send(client_socket_fd, msg, sizeof(msg), 0);
      	    close(client_socket_fd);
        }
    }
    return EXIT_SUCCESS;
}

