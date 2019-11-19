#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    m_server_addr.sin_port = htons(7777);
    memset(&m_server_addr.sin_zero, 0, sizeof(m_server_addr.sin_zero));

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_request_clicked()
{
    ui->textBrowser_info->append("Clicked request!");

    Client client;
    if (!client.Connect(&m_server_addr))
        return;

    char buf[256];
    client.Read(buf, sizeof(buf) - 1);

    ui->textBrowser_info->append(QString(buf));
}
