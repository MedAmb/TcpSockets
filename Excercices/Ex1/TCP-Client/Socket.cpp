#include "Socket.h"


Socket::Socket(int domain, int type, int protocol, int port, bool reuse, socketType sockettype):
m_domain(domain),
m_type(type),
m_protocol(protocol),
m_socketType(sockettype)
{
    m_socketId = socket(m_domain, m_type, m_protocol);

    if(m_socketId < 0)
    {
        std::cout << "Error : couldn't create socket !!" << std::endl;
    }
    else
    {
        if (reuse)
        {
            int enable = 1;
            if (setsockopt(m_socketId, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            {
                std::cout << "Error : socket reusisability failed !!" << std::endl;
            }
        }
        m_socketAdress.sin_family = m_domain;
        m_socketAdress.sin_port = htons(port);
    }

    if(m_socketType == SERVER)
    {
        m_success = createServer();
    }
    else
    {
        m_success = createClient();
    }
}

int Socket::m_bind(uint32_t adress)
{
    m_socketAdress.sin_addr.s_addr = adress;
    int s_return = bind(m_socketId, (struct sockaddr *)(&m_socketAdress), sizeof(sockaddr_in));

    if(s_return < 0)
    {
        std::cout << "Error : couldn't bind socket !!" << std::endl;
    }
    return s_return;    
}

int Socket::m_connect(uint32_t adress)
{
    m_socketAdress.sin_addr.s_addr = adress;
    int s_return = connect(m_socketId, (struct sockaddr *)(&m_socketAdress), sizeof(sockaddr_in));

    if(s_return < 0)
    {
        std::cout << "Error : couldn't connect to server !!" << std::endl;
    }
    return s_return;
}

int Socket::m_accept()
{
    auto addressLength = sizeof(sockaddr);
    m_connection = accept(m_socketId, (struct sockaddr *)&m_socketAdress, (socklen_t *)&addressLength);

    if(m_connection < 0)
    {
        std::cout << "Error : connection failed !!" << std::endl;
    }
    return m_connection;
}

int Socket::m_listen(int maxConnectionAllowed)
{
    int s_return = listen(m_socketId, maxConnectionAllowed);

    if (s_return < 0)
    {
        std::cout << "Error : listening failed !!" << std::endl;
    }

    return s_return;
}

int Socket::createServer()
{
    if (m_socketId < 0)
    {
        return -1;
    }

    if(m_bind(INADDR_ANY) < 0)
    {
        return -1;
    }

    if (m_listen(2) < 0)
    {
        return -1;
    }

    if (m_accept() < 0)
    {
        return -1;
    }

    return 0;
}

int Socket::createClient()
{
    if (m_socketId < 0)
    {
        return -1;
    }
    uint32_t address = 0;
    inet_pton(AF_INET, "127.0.0.1", &address);

    if(m_connect(address) < 0)
    {
        return -1;
    }

    return 0;
}

int Socket::m_send(const std::string& message)
{
    int s_return;

    if(m_socketType == SERVER)
    {   
        s_return = send(m_connection, message.c_str(), message.size(), 0);
    }
    else
    {
        s_return = send(m_socketId, message.c_str(), message.size(), 0);
    }
    

    if(s_return < 0)
    {
        std::cout << "Error : sending failed !!" << std::endl;
    }
    return s_return;
}

int Socket::m_receive()
{
    int s_return;
    if(m_socketType == SERVER)
    {
        s_return = read(m_connection, m_messageReceived, 100);
    } 
    else 
    {
        s_return = read(m_socketId, m_messageReceived, 100);
    }
    
    if(s_return < 0)
    {
        std::cout << "Error : reception failed !!" << std::endl;
    }
    return s_return;
}

char * Socket::m_getMessage()
{
    memset(m_messageReceived, 0, (size_t)100);
    m_receive();
    return m_messageReceived;
}

void Socket::m_close()
{
    close(m_connection);
    close(m_socketId);
}
