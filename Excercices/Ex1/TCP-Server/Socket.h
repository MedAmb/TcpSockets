#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
# include <string.h>

typedef enum
{
    SERVER,
    CLIENT
} socketType;

class Socket
{
private:
    int m_port;
    int m_domain;
    int m_type;
    int m_protocol;
    socketType m_socketType;
    sockaddr_in m_socketAdress;
public:
    int m_socketId;
        int m_connection;
        int m_success;
    char m_messageReceived[100];
public:
    Socket(int domain, int type, int protocol, int port, bool reuse, socketType sockettype);

    int m_send(const std::string& message);

    char * getMessage();

    void m_close();

private:
    Socket() = delete;

    int m_bind(uint32_t adress);

    int m_connect(uint32_t adress);

    int m_accept();

    int m_listen(int maxConnectionsAllowed);

    int m_receive();


    int createServer();

    int createClient();
};