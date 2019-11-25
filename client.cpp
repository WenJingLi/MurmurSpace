#include "client.h"
#include <QDebug>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

Client::Client()
    : m_client_sock{socket(AF_INET, SOCK_STREAM, 0)}
{
    if (-1 == m_client_sock)
    {
        qDebug("Failed to create client socket fd!");
    }
}

Client::~Client()
{
    CloseClientSocket();
}

bool Client::Connect(const sockaddr_in *addr)
{
    if (-1 == m_client_sock)
    {
        qDebug("Invalid client socket fd!");
        return false;
    }
    auto result = ::connect(m_client_sock, (sockaddr*)addr, sizeof(sockaddr_in));
    if (0 != result)
    {
        qDebug("Failed to execute connect!");
        return false;
    }
    return true;
}

ssize_t Client::Write(const void *buf, size_t bytes)
{
    if (-1 == m_client_sock)
    {
        qDebug("Invalid client socket fd!");
        return -1;
    }
    return write(m_client_sock, buf, bytes);
}

ssize_t Client::Read(void *buf, size_t bytes)
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
    ssize_t size = read(m_client_sock, buf, bytes);
    emit recvMsg();
    return size;
}

ssize_t Client::Read(QString &buf)
{
    if (-1 == m_client_sock)
    {
        qDebug("Client::Read --- Invalid client socket fd!");
        return -1;
    }

    buf.clear();
    char buffer[256];
    memset(buffer, 0, 256);
    ssize_t size = read(m_client_sock, buffer, 256);
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
        close(m_client_sock);
        m_client_sock = -1;
    }
}
