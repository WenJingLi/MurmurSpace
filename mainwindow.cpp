#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "clientthread.h"

#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client_thread{new ClientThread(this)}
{
    ui->setupUi(this);

    ui->textEdit_msg->installEventFilter(this);

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
    ui->textEdit_msg->clear();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    Q_ASSERT(obj == ui->textEdit_msg);
    if (e->type() == QEvent::KeyPress)
    {

        QKeyEvent *event = static_cast<QKeyEvent*>(e);

        if (event->key() == Qt::Key_Return && (event->modifiers() & Qt::ControlModifier))
        {
            ui->textEdit_msg->append("");
            return true;
        }

        if (event->key() == Qt::Key_Return)
        {
            on_pushButton_send_clicked();
            return true;
        }
    }
    return false;
}
