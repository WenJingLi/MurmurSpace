#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>

struct sockaddr_in;

class Client
{
public:
    Client();
    ~Client();

    bool Connect(const sockaddr_in* addr);
    ssize_t Write(const void* buf, size_t bytes);
    ssize_t Read(void* buf, size_t bytes);

private:
    int m_client_sock;
};

#endif // CLIENT_H
