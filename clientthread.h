#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <netinet/in.h>

class Client;

class ClientThread : public QThread
{
    Q_OBJECT
signals:
    void message(const QString& msg);
    void sendRequest();

public:
    ClientThread(QObject* parent);
    virtual ~ClientThread() override;

    virtual void run() override;

    bool SendRequest();

private slots:
    void transmitMsg();
    void readMsg();

private:
    Client* m_client;
    sockaddr_in m_server_addr;
    QString m_msg;
    bool m_receiving;
};

#endif // CLIENTTHREAD_H