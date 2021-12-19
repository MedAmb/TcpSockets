#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <iostream>

using namespace std;

int main()
{
    int serverSock = 0;
    

    struct sockaddr_in addr;
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

   
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(serverSock, (struct sockaddr *)&addr, sizeof(addr));
    cout<< "Connected to server!"<< endl;
    char buffer[1024]={'h', 'e', 'l', 'l', 'o', '.'};
    write(serverSock, buffer, strlen(buffer));
    cout<<"Message sent! with buffer in Client"<< buffer << endl;

    close(serverSock);
   
    cout<<"Socket closed."<< endl;
	
}


