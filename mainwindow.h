#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ClientThread;

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
    void on_pushButton_send_clicked();

private:
    bool eventFilter(QObject *obj, QEvent *e);

    Ui::MainWindow *ui;
    ClientThread* m_client_thread;

};
#endif // MAINWINDOW_H
