#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "clientthread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client_thread{new ClientThread(this)}
{
    ui->setupUi(this);

    connect(m_client_thread, &ClientThread::message, ui->textBrowser_info, &QTextBrowser::append);

    if (nullptr != m_client_thread)
    {
        m_client_thread->start();
    }
}

MainWindow::~MainWindow()
{
    if (nullptr != m_client_thread)
    {
        delete m_client_thread;
        m_client_thread = nullptr;
    }

    delete ui;
}

void MainWindow::on_pushButton_send_clicked()
{
    if (nullptr == m_client_thread)
    {
        qDebug("The client is nullptr!");
        return;
    }
    if (!m_client_thread->isRunning())
    {
        qDebug("The client thread isn't running!");
        return;
    }

    m_client_thread->SendMsg(ui->textEdit_msg->toPlainText());
}
