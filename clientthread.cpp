#include "clientthread.h"
#include "client.h"

#ifdef __unix__
#   include <sys/socket.h>
#   include <arpa/inet.h>
#endif

ClientThread::ClientThread(QObject* parent)
    : QThread(parent)
    , m_client{new Client()}
    , m_msg{""}
    , m_receiving{false}
{
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
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
    emit message(QString("Client started."));
    connect(m_client, &Client::recvMsg, this, &ClientThread::transmitMsg);
    connect(this, &ClientThread::sendRequest, this, &ClientThread::readMsg);
    exec();
}

bool ClientThread::SendMsg(const QString& msg)
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
            emit message("Failed to connect server!");
            return false;
        }
        m_client->Write(msg.toStdString().c_str(), msg.size());
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
