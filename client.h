#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

struct sockaddr_in;

class Client : public QObject
{
    Q_OBJECT
signals:
    void recvMsg();

public:
    Client();
    virtual ~Client();

    bool createSocket();
    bool connect(const sockaddr_in* addr);
    int read(char *buf);
    bool sendMsg(const char *msg, const size_t &len);
    void closeClientSocket();

private:
    int m_client_sock;
};

#endif // CLIENT_H
