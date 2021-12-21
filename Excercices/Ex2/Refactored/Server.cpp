#include "Socket.h"
#include <string.h>
#include <ctime>


void diplayData(Socket &server)
{
    uint8_t * (data) = (uint8_t *)server.m_messageReceived;

    std::cout << "| > Message received : "
              << "Frame number : " << (uint32_t)((*(data) << 24) |  (*(data + 1) << 16) |  (*(data + 2) << 8) |  (*(data + 3))) << " | "
              << "PID : " << (uint16_t)((*(data + 4) << 8) | (*(data + 5))) << " | "
              << "Timestamp : " << (uint32_t)((*(data + 6) << 24) |  (*(data + 7) << 16) |  (*(data + 8) << 8) |  (*(data + 9))) << " | "
              << "Data : " << *(data + 10) << *(data + 11)  << *(data + 12)  << *(data + 13)  << *(data + 14) << std::endl;
}

int main()
{
    Socket ServerSocket(AF_INET, SOCK_STREAM, 0, 3456, true, SERVER);

    int frameNumber, PID, timeStamp;
    char * data;

    if(ServerSocket.m_success == -1)
        exit(EXIT_FAILURE);

    do
    {
        ServerSocket.m_getMessage();
        diplayData(ServerSocket);
        std::string response2 = " Acknowledged.";
        ServerSocket.m_send(response2);

    }while (true);

    ServerSocket.m_close();
}