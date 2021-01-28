#include "clientthread.h"
#include "client.h"
#include <thread>

#ifdef __unix__
#   include <sys/socket.h>
#   include <arpa/inet.h>
#endif

extern const unsigned int RECV_SIZE = 512;

ClientThread::ClientThread(QObject* parent)
    : QThread(parent)
    , m_client{new Client()}
    , m_msg{new char[RECV_SIZE]}
{
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = inet_addr("192.168.162.24");
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
//    connect(this, &ClientThread::sendRequest, this, &ClientThread::readMsg);

    if (nullptr == m_client)
    {
        qDebug("ClientThread::run --- m_clent is nullptr!");
        return;
    }

    if (m_client->createSocket())
    {
        if (!m_client->connect(&m_server_addr))
        {
            m_client->closeClientSocket();
            emit message("Failed to connect server!");
            return;
        }
    }

    std::thread([&]()
    {
        while (true)
        {
            if (!readMsg())
                return;
        }
    }).detach();

    exec();
}

bool ClientThread::SendMsg(const QString& msg)
{
    if (nullptr == m_client)
    {
        qDebug("ClientThread::SendRequest --- m_clent is nullptr!");
        return false;
    }

    m_client->sendMsg(msg.toStdString().c_str(), msg.toStdString().size());
    return true;
}

void ClientThread::transmitMsg()
{
    emit message(m_msg);
}

bool ClientThread::readMsg()
{
    if (nullptr == m_client)
    {
        qDebug("ClientThread::readMsg --- m_clent is nullptr!");
        return false;
    }

    if (-1 == m_client->read(m_msg))
    {
        qDebug("ClientThread::readMsg --- Read message failed!");
        m_client->closeClientSocket();
        return false;
    }
    emit message(m_msg);
    return true;
}
