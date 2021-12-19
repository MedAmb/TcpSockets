
//Using Select for MultiClient - Server Communication on Linux

#include <stdio.h>  
#include <string.h>
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>
#include <arpa/inet.h> 
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h>  
     
#define TRUE   1  
#define FALSE  0  
#define PORT 8888  
     
int main(int argc , char *argv[])   
{   
    int opt = TRUE;   
    int master_socket , addrlen , client_soc , client_socket[10] , max_clients = 10 , activity, i , valread , sd;   
    int max_sd;   
    struct sockaddr_in address;   
         
    char buffer[1025];
         
    fd_set readfds;   
    char *message = "Acknowledgement message send from Server \r\n";   
     
    //initialise all client_socket[] to 0 so not checked  
    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    }   
         
    //create a master socket  
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
     
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )   
    {   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }   
     
    //type of socket created  
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( PORT );   
         
    //bind the socket to localhost port 8888  
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", PORT);   
         
    //Allow maximum of 2 pending connections
    if (listen(master_socket, 2) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
         
    //accept the incoming connection  
    addrlen = sizeof(address);   
    puts("Waiting for connections ...");   
         
    while(TRUE)   
    {
        //clear the socket set  
        FD_ZERO(&readfds);   
     
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   
        printf("Value of master socket is %d \n",master_socket);
             
        //add child sockets to set  
        for ( i = 0 ; i < max_clients ; i++)   
        {  
		 
	    printf("In for loop to add child socket  \n");   
  
            sd = client_socket[i];   
	
     		
            if(sd > 0)   
	    {
		FD_SET( sd , &readfds);   
	    }
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
	    {
                max_sd = sd;   
            }
        }   
    	 
        
	printf("BEFORE SELECT \n");
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
	printf("AFTER SELECT \n");
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   
             
        if (FD_ISSET(master_socket, &readfds))   
        {   
            if ((client_soc = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            //inform user of socket number - used in send and receive commands  
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , client_soc , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   
           
            //send new connection acknowledgement message  
            if( send(client_soc, message, strlen(message), 0) != strlen(message) )   
            {   
                perror("send");   
            }   
                 
            puts("acknowledgement message sent successfully from Server");   
                 
            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = client_soc;   
                    break;   
                }   
            }   
        }   
             
        //else its some IO operation on some other socket 
        for (i = 0; i < max_clients; i++)   
        {  
	     
	    sd = client_socket[i];   
                 
	    printf("In Forloopio is done on client socket, sd is %d \n", sd);   
        
            if (FD_ISSET( sd , &readfds)) //if send receive on client socket  
            {  
		 
  	        printf("Client socket for reading is set\n");   
        
                //Check if it was for closing , and also read the  
                //incoming message  
                if ((valread = read( sd , buffer, 1024)) == 0)   
                {   
		    printf("In if condition read, no bytes return from read \n");
                     
                    //Close the socket and mark as 0 in list for reuse  
                    close( sd );   
                    client_socket[i] = 0;   
                }   
                     
                //Echo back the message that came in  
                else 
                {  
		 
		    printf("In else condition read buffer is %s \n", buffer);
                   
                    buffer[valread] = '\0';
		    send(sd , buffer , strlen(buffer) , 0 );   
		    
                }   
            }   
        }   
    }   
         
    return 0;   
}   

