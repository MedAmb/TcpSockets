#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
# include <string.h>
#include <chrono>
#include <thread>

typedef enum
{
    SERVER,
    CLIENT
} socketType;

typedef enum
{
    SINGLE,
    MULTIPLE
} serverType;

class Socket
{
private:
    int m_port;
    int m_domain;
    int m_type;
    int m_protocol;
    socketType m_socketType;
    sockaddr_in m_socketAdress;
    serverType m_serverType;
    int m_clientSocketIdTemp;
public:
    char m_messageReceived[100];
    int m_success;
    int m_masterSocketId;
    int m_clientSocketId[10];

public:
    Socket(int domain, int type, int protocol, int port, bool reuse, socketType sockettype, bool waitForServer = false, serverType servertype=SINGLE);

    int m_send(const std::string& message, int index=0);

    char * m_getMessage(int index=0);

    void m_closeClient(int index=0);

    void m_closeMaster();

    void m_close();

    int m_accept();

    int m_getPeerName(int index);

protected:
    Socket() = delete;

    int m_bind(uint32_t adress);

    int m_connect(uint32_t adress);

    int m_listen(int maxConnectionsAllowed);

    int m_receive(int index);

    int createServer();

    int createClient(bool waitForServer);

    int createServerMultipleClients();
};