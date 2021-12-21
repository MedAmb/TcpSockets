#include "Socket.h"
#include <string.h>
#include <pthread.h>
#include <thread>
#include <chrono>

std::string response = "Hello From Client";

void commandInputFunc()
{   
    do
    {
        std::cin >> response;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while (strncmp(response.c_str(), "quit", 4) != 0);

}

int main()
{
    Socket clientSocket(AF_INET, SOCK_STREAM, 0, 9988, false, CLIENT);

    if(clientSocket.m_success == -1)
    {
        exit(EXIT_FAILURE);
    }

    std::cout << "================================ TCP Client Open =================================\n";

    clientSocket.m_send(response);
    std::cout << "\n\n| > \t\tServer reponse\n" << clientSocket.m_getMessage() << std::endl;

    std::thread commantInput(commandInputFunc);

    do
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        clientSocket.m_send(response);
        std::cout << "\n\n| > \t\tServer reponse\n" << clientSocket.m_getMessage() << std::endl;
    }while (strncmp(response.c_str(), "quit", 4) != 0);

    std::cout << "\t\tServer reponse\n" << clientSocket.m_getMessage() << std::endl;

    clientSocket.m_close();

    commantInput.join();
}


 