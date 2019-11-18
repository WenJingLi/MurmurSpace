#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <netinet/in.h>

class Client;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_request_clicked();

private:
    Ui::MainWindow *ui;
    Client* m_client;
    sockaddr_in m_server_addr;

};
#endif // MAINWINDOW_H
