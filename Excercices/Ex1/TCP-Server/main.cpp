#include "Socket.h"
#include <string.h>


int main()
{
    Socket ServerSocket(AF_INET, SOCK_STREAM, 0, 9988, true, SERVER);

    if(ServerSocket.m_success == -1)
        exit(EXIT_FAILURE);

    do
    {
        std::string response = "| > Listnening..., type quit to terminate theserver\n";
        ServerSocket.m_send(response);
        std::cout << "| > The message was: " << ServerSocket.getMessage() << std::endl;
        std::string response2 = "| > Acknowledged.\n";
        ServerSocket.m_send(response2);
    }while (strncmp(ServerSocket.m_messageReceived, "quit", 4) != 0);


    std::string response = "| > Server shutting down...\n";
    ServerSocket.m_send(response);
    std::cout << "| > Server shutting down...\n" << std::endl;

    ServerSocket.m_close();
}