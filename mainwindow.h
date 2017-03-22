#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QList>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_btnConnect_clicked();
    void on_btnSend_clicked();
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    Ui::MainWindow *ui;
    QSerialPort *port;
};

#endif // MAINWINDOW_H
