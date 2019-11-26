#include "clientthread.h"
#include "client.h"

#include <sys/socket.h>
#include <arpa/inet.h>

ClientThread::ClientThread(QObject* parent)
    : QThread(parent)
    , m_client{new Client()}
    , m_msg{""}
    , m_receiving{false}
{
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    m_server_addr.sin_port = htons(7777);
    memset(&m_server_addr.sin_zero, 0, sizeof(m_server_addr.sin_zero));
}

ClientThread::~ClientThread()
{
    if (nullptr != m_client)
    {
        delete m_client;
        m_client = nullptr;
    }

    quit();
    wait();
}

void ClientThread::run()
{
    emit message(QString("Start client thread."));
    connect(m_client, &Client::recvMsg, this, &ClientThread::transmitMsg);
    connect(this, &ClientThread::sendRequest, this, &ClientThread::readMsg);
    exec();
}

bool ClientThread::SendRequest()
{
    if (m_receiving)
    {
        emit message("Receiving message from server, please wait.");
        return false;
    }

    if (nullptr == m_client)
    {
        qDebug("ClientThread::SendRequest --- m_clent is nullptr!");
        return false;
    }

    if (m_client->CreateSocket())
    {
        if (!m_client->Connect(&m_server_addr))
        {
            m_client->CloseClientSocket();
            return false;
        }
        m_receiving = true;
        emit sendRequest();
    }
    return true;
}

void ClientThread::transmitMsg()
{
    if (!m_msg.isEmpty())
    {
        emit message(m_msg);
    }
    else
    {
        emit message("Failed to receive message!");
    }
}

void ClientThread::readMsg()
{
    if (nullptr == m_client)
    {
        qDebug("ClientThread::readMsg --- m_clent is nullptr!");
        return;
    }

    if (-1 == m_client->Read(m_msg))
    {
        qDebug("ClientThread::readMsg --- Read message failed!");
    }
    m_client->CloseClientSocket();
    m_receiving = false;
}
