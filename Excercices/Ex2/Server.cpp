#include "Socket.h"
#include <string.h>
#include <ctime>
#include <sys/time.h>

void diplayData(Socket &server)
{
    uint8_t *(data) = (uint8_t *)server.m_messageReceived;

    std::cout << "| > Message received : "
              << "Frame number : " << (uint32_t)((*(data) << 24) | (*(data + 1) << 16) | (*(data + 2) << 8) | (*(data + 3))) << " | "
              << "PID : " << (uint16_t)((*(data + 4) << 8) | (*(data + 5))) << " | "
              << "Timestamp : " << (uint32_t)((*(data + 6) << 24) | (*(data + 7) << 16) | (*(data + 8) << 8) | (*(data + 9))) << " | "
              << "Data : " << *(data + 10) << *(data + 11) << *(data + 12) << *(data + 13) << *(data + 14) << std::endl;
}

int main()
{
    Socket ServerSocket(AF_INET, SOCK_STREAM, 0, 3456, true, SERVER, false, MULTIPLE);

    int frameNumber, PID, timeStamp;
    char *data;

    int activity, maxSocketId, socketId;

    fd_set readfds;

    if (ServerSocket.m_success == -1)
        exit(EXIT_FAILURE);

    do
    {

        FD_ZERO(&readfds);

        FD_SET(ServerSocket.m_masterSocketId, &readfds);
        maxSocketId = ServerSocket.m_masterSocketId;

        for (int i = 0; i < 10; i++)
        {
            if (ServerSocket.m_clientSocketId[i] > 0)
                FD_SET(ServerSocket.m_clientSocketId[i], &readfds);

            if (ServerSocket.m_clientSocketId[i] > maxSocketId)
                maxSocketId = ServerSocket.m_clientSocketId[i];
        }

        activity = select(maxSocketId + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            std::cout << "Error : Socket selection !!" << std::endl;
        }

        if (FD_ISSET(ServerSocket.m_masterSocketId, &readfds))
        {
            socketId = ServerSocket.m_accept();
            if (socketId < 0)
            {
                std::cout << "Error : Establishing connection !!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::cout << "New Connection Established, Socket ID = " << socketId << std::endl;

            for (int i = 0; i < 10; i++)
            {
                if (ServerSocket.m_clientSocketId[i] == 0)
                {
                    ServerSocket.m_clientSocketId[i] = socketId;
                    std::string response2 = "Welcome !!";
                    ServerSocket.m_send(response2, i);
                    break;
                }
            }
        }

        for (int i = 0; i < 10; i++)
        {

            if (FD_ISSET(ServerSocket.m_clientSocketId[i], &readfds))
            {
                if (ServerSocket.m_getMessage(i) == 0)
                {
                    ServerSocket.m_getPeerName(i);
                }

                else
                {
                    diplayData(ServerSocket);
                    std::string response2 = "Acknowleged !";
                    ServerSocket.m_send(response2, i);
                }
            }
        }
    } while (true);

    ServerSocket.m_close();
}