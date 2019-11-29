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
    CloseClientSocket();
}

bool Client::CreateSocket()
{
    m_client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_client_sock)
    {
        qDebug("Client::CreateSocket --- Failed to create client socket fd!");
        return false;
    }
    return true;
}

bool Client::Connect(const sockaddr_in *addr)
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

int Client::Write(const void *buf, size_t bytes)
{
    if (-1 == m_client_sock)
    {
        qDebug("Client::Write --- Invalid client socket fd!");
        return -1;
    }
#ifdef __unix__
    return write(m_client_sock, buf, bytes);
#else
    return send(m_client_sock, (const char*)buf, bytes, 0);
#endif
}

int Client::Read(void *buf, size_t bytes)
{
    if (-1 == m_client_sock)
    {
        qDebug("Client::Read --- Invalid client socket fd!");
        return -1;
    }
    if (nullptr == buf)
    {
        qDebug("Client::Read --- buf is nullptr!");
        return -1;
    }

#ifdef __unix__
    auto size = read(m_client_sock, buf, bytes);
#else
    auto size = recv(m_client_sock, (char*)buf, bytes, 0);
#endif

    emit recvMsg();
    return size;
}

int Client::Read(QString &buf)
{
    if (-1 == m_client_sock)
    {
        qDebug("Client::Read --- Invalid client socket fd!");
        return -1;
    }

    buf.clear();
    char buffer[256];
    memset(buffer, 0, 256);
#ifdef __unix__
    auto size = read(m_client_sock, buffer, 256);
#else
    auto size = recv(m_client_sock, buffer, 255, 0);
#endif
    if (-1 != size)
    {
        buf = QString(buffer);
    }
    emit recvMsg();
    return size;
}

void Client::CloseClientSocket()
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
