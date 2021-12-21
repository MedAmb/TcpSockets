#include "Socket.h"


Socket::Socket(int domain, int type, int protocol, int port, bool reuse, socketType sockettype, bool waitForServer = false, serverType servertype=SINGLE):
m_domain(domain),
m_type(type),
m_protocol(protocol),
m_socketType(sockettype),
m_serverType(servertype)
{
    m_masterSocketId = socket(m_domain, m_type, m_protocol);

    if(m_masterSocketId < 0)
    {
        std::cout << "Error : couldn't create socket !!" << std::endl;
    }
    else
    {
        if (reuse)
        {
            int enable = 1;
            if (setsockopt(m_masterSocketId, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            {
                std::cout << "Error : socket reusisability failed !!" << std::endl;
            }
        }
        m_socketAdress.sin_family = m_domain;
        m_socketAdress.sin_port = htons(port);
    }

    if(m_socketType == SERVER)
    {
        if(m_serverType == SINGLE)
            m_success = createServer();
        else
            m_success = createServerMultipleClients();
    }
    else
    {
        m_success = createClient(waitForServer);
    }
}

int Socket::m_bind(uint32_t adress)
{
    m_socketAdress.sin_addr.s_addr = adress;
    int s_return = bind(m_masterSocketId, (struct sockaddr *)(&m_socketAdress), sizeof(sockaddr_in));

    if(s_return < 0)
    {
        std::cout << "Error : couldn't bind socket !!" << std::endl;
    }
    return s_return;    
}

int Socket::m_connect(uint32_t adress)
{
    m_socketAdress.sin_addr.s_addr = adress;
    int s_return = connect(m_masterSocketId, (struct sockaddr *)(&m_socketAdress), sizeof(sockaddr_in));

    if(s_return < 0)
    {
        std::cout << "Error : couldn't connect to server !!" << std::endl;
    }
    return s_return;
}

int Socket::m_accept()
{
    auto addressLength = sizeof(sockaddr);
    m_clientSocketIdTemp = accept(m_masterSocketId, (struct sockaddr *)&m_socketAdress, (socklen_t *)&addressLength);

    if(m_clientSocketIdTemp < 0)
    {
        std::cout << "Error : connection failed !!" << std::endl;
    }
    return m_clientSocketIdTemp;
}

int Socket::m_getPeerName(int index)
{
    auto addressLength = sizeof(sockaddr);
    int s_return;
    s_return = getpeername(m_clientSocketId[index], (struct sockaddr *)&m_socketAdress, (socklen_t *)&addressLength);

    std::cout << "Host disconnected, Socket Id = " << m_clientSocketId[index] << std::endl;

    m_clientSocketId[index] = 0;

    m_closeClient(index);

    return s_return;
}

int Socket::m_listen(int maxConnectionAllowed)
{
    int s_return = listen(m_masterSocketId, maxConnectionAllowed);

    if (s_return < 0)
    {
        std::cout << "Error : listening failed !!" << std::endl;
    }

    return s_return;
}

int Socket::createServer()
{
    if (m_masterSocketId < 0)
    {
        return -1;
    }

    if(m_bind(INADDR_ANY) < 0)
    {
        return -1;
    }

    if (m_listen(3) < 0)
    {
        return -1;
    }

    std::cout << "Waiting for client to connect ..." << std::endl;

    if (m_accept() < 0)
    {
        return -1;
    }

    std::cout << "Connection established ..." << std::endl;

    return 0;
}

int Socket::createServerMultipleClients()
{
    for(int i = 0; i < 10; i++)
        m_clientSocketId[i] = 0;

    if (m_masterSocketId < 0)
    {
        return -1;
    }

    if(m_bind(INADDR_ANY) < 0)
    {
        return -1;
    }

    if (m_listen(3) < 0)
    {
        return -1;
    }

    std::cout << "Waiting for clients to connect ..." << std::endl;

    return 0;
}

int Socket::createClient(bool waitForServer)
{
    if (m_masterSocketId < 0)
    {
        return -1;
    }
    uint32_t address = 0;
    inet_pton(AF_INET, "127.0.0.1", &address);

    do
    {
        std::cout << "Waiting for Server .... \n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }while((m_connect(address) < 0) && waitForServer);

    return 0;
}

int Socket::m_send(const std::string& message, int index=0)
{
    int s_return;
    
    if(m_socketType == SERVER)
    {   
        s_return = send(m_clientSocketId[index], message.c_str(), message.size(), 0);
    }
    else
    {
        s_return = send(m_masterSocketId, message.c_str(), 16, 0);
    }
    

    if(s_return < 0)
    {
        std::cout << "Error : sending failed !!" << std::endl;
    }
    return s_return;
}

int Socket::m_receive(int index)
{
    int s_return;
    if(m_socketType == SERVER)
    {
        s_return = read(m_clientSocketId[index], m_messageReceived, 100);
    } 
    else 
    {
        s_return = read(m_masterSocketId, m_messageReceived, 100);
    }
    
    if(s_return < 0)
    {
        std::cout << "Error : reception failed !!" << std::endl;
    }
    return s_return;
}

char * Socket::m_getMessage(int index=0)
{
    memset(m_messageReceived, 0, (size_t)100);
    m_receive(index);
    return m_messageReceived;
}

void Socket::m_closeClient(int index=0)
{
    close(m_clientSocketId[index]);

}

void Socket::m_closeMaster()
{
    close(m_masterSocketId);;
}

void Socket::m_close()
{
    m_closeClient();
    m_closeMaster();
}
    