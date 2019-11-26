#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <sys/types.h>

struct sockaddr_in;

class Client : public QObject
{
    Q_OBJECT
signals:
    void recvMsg();

public:
    Client();
    virtual ~Client();

    bool CreateSocket();
    bool Connect(const sockaddr_in* addr);
    ssize_t Write(const void* buf, size_t bytes);
    ssize_t Read(void* buf, size_t bytes);
    ssize_t Read(QString& buf);
    void CloseClientSocket();

private:

    int m_client_sock;
};

#endif // CLIENT_H
