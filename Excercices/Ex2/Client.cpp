#include "Socket.h"
#include <string>
#include <pthread.h>
#include <ctime>

uint8_t frame[16];


void buildFrame()
{
    static int frameNumber = 0;
    time_t timeStamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    frame[0] = (frameNumber & 0xff000000) >> 24;
    frame[1] = (frameNumber & 0x00ff0000) >> 16;
    frame[2] = (frameNumber & 0x0000ff00) >> 8;
    frame[3] = (frameNumber & 0xff0000ff);

    frame[6] = (timeStamp & 0xff000000) >> 24;;
    frame[7] = (timeStamp & 0x00ff0000) >> 16;
    frame[8] = (timeStamp & 0x0000ff00) >> 8;
    frame[9] = (timeStamp & 0xff0000ff);

    frameNumber++;
}


int main()
{
    int PID = ::getpid();

    frame[4] = (PID & 0xff00) >> 8;
    frame[5] = (PID & 0x00ff);

    frame[10] = 'H';
    frame[11] = 'e';
    frame[12] = 'l';
    frame[13] = 'l';
    frame[14] = 'o';
    frame[15] = 0;

    Socket clientSocket(AF_INET, SOCK_STREAM, 0, 3456, false, CLIENT, true);
    if (clientSocket.m_success == -1)
    {
        exit(EXIT_FAILURE);
    }

    buildFrame();
    clientSocket.m_send(std::string((char *)frame, 16));
    std::cout << "| > Server reponse : " << clientSocket.m_getMessage() << std::endl;

    do
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        buildFrame();
        clientSocket.m_send(std::string((char *)frame, 16));
        std::cout << "| > Server reponse : " << clientSocket.m_getMessage() << std::endl;
    } while (true);

    clientSocket.m_close();
}
