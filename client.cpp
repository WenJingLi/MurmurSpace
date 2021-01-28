#include "client.h"
#include <QDebug>

#ifdef __unix__
#   include <unistd.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#else
#   include <WinSock2.h>
#   pragma comment(lib, "ws2_32.lib")
#endif

extern const unsigned int RECV_SIZE;

Client::Client()
    : m_client_sock{-1}
{
#ifndef __unix__
    WSADATA wsa_data;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wsa_data))
    {
        qDebug("Client::Client --- Failed to init WinSock!(WSAStartup)");
    }
#endif
}

Client::~Client()
{
    closeClientSocket();
}

bool Client::createSocket()
{
    m_client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_client_sock)
    {
        qDebug("Client::CreateSocket --- Failed to create client socket fd!");
        return false;
    }
    return true;
}

bool Client::connect(const sockaddr_in *addr)
{
    if (-1 == m_client_sock)
    {
        qDebug("Client::Connect --- Invalid client socket fd!");
        return false;
    }
    auto result = ::connect(m_client_sock, (sockaddr*)addr, sizeof(sockaddr_in));
    if (0 != result)
    {
        qDebug("Client::Connect --- Failed to execute connect!");
        return false;
    }

    return true;
}

bool Client::sendMsg(const char *msg, const size_t &len)
{
    if (-1 == m_client_sock)
    {
        qDebug("Client::sendMsg --- Invalid client socket fd!");
        return -1;
    }
#ifdef __unix__
    return write(m_client_sock, msg, len);
#else
    return send(m_client_sock, msg, len, 0);
#endif
}

int Client::read(char *buf)
{
    if (-1 == m_client_sock)
    {
        qDebug("Client::Read --- Invalid client socket fd!");
        return -1;
    }

    memset(buf, 0, RECV_SIZE);
#ifdef __unix__
    auto size = recv(m_client_sock, buf, RECV_SIZE, MSG_NOSIGNAL);
#else
    auto size = recv(m_client_sock, buf, RECV_SIZE, 0);
#endif

    if (0 >= size)
    {
        std::string m{"socket recv failed"};
        memcpy(buf, m.data(), m.size());
    }
    emit recvMsg();
    return size;
}

void Client::closeClientSocket()
{
    if (-1 != m_client_sock)
    {
#ifdef __unix__
        close(m_client_sock);
#else
        closesocket(m_client_sock);
#endif
        m_client_sock = -1;
    }
}
