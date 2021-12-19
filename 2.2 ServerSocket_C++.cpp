#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>

using namespace std;

int main()
{
    int server = 0, client = 0;//Linux specifix
    struct sockaddr_in serverAddr;//linux specific

    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);

    bind(server, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    
    cout<<"Listening for incoming connections..."<<endl;

    char buffer[1024] = { 0 };
  
	
    client = accept(server, (struct sockaddr*)NULL, NULL);
    cout<<"Client connected!" << endl;
    read(client, buffer, sizeof(buffer)-1);

    cout<<"client says: buffer is " << buffer <<endl;
        	
    memset(buffer, 0, sizeof(buffer));

    close(client) ;
    cout<<"Client disconnected" << endl;
	
}


