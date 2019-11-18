#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client{new Client()}
{
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    m_server_addr.sin_port = htons(7777);
    memset(&m_server_addr.sin_zero, 0, sizeof(m_server_addr.sin_zero));

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (nullptr != m_client)
    {
        delete m_client;
        m_client = nullptr;
    }
    delete ui;
}


void MainWindow::on_pushButton_request_clicked()
{
    ui->textBrowser_info->append("Clicked request!");

    if (nullptr == m_client)
    {
        qDebug("m_client is nullptr!");
        return;
    }

    if (!m_client->Connect(&m_server_addr))
        return;

    char buf[256];
    m_client->Read(buf, sizeof(buf) - 1);

    ui->textBrowser_info->append(QString(buf));
}
