#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    for(int  i = 0; i < portList.size(); i++) {
        QSerialPortInfo info = portList.at(i);
        ui->cbPorts->addItem(info.portName());
    }
    ui->btnSend->setEnabled(false);
    ui->txtData->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete port;
}

void MainWindow::on_btnConnect_clicked()
{
    QString selectedPortName = ui->cbPorts->currentText();
    qDebug() << "Selected Port: " << selectedPortName << endl;
    port = new QSerialPort(selectedPortName);
    connect(port, &QSerialPort::readyRead, this, &MainWindow::handleReadyRead);
    connect(port, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)> (&QSerialPort::error), this, &MainWindow::handleError);

    if(port->open(QIODevice::ReadWrite)) {
        ui->btnConnect->setEnabled(false);
        ui->cbPorts->setEnabled(false);
        ui->txtData->setEnabled(true);
        ui->btnSend->setEnabled(true);
        QMessageBox mbox;
        mbox.setText(selectedPortName + " opened.");
        mbox.exec();
    } else {
        QMessageBox mbox;
        mbox.setText("A problem occured while trying to open port " + selectedPortName);
        mbox.exec();
    }
}

void MainWindow::handleReadyRead() {
    std::string str = port->readAll().toStdString();
    ui->lblReceivedData->setText(QString::fromStdString(str));
}

void MainWindow::handleError(QSerialPort::SerialPortError error) {
    if(error == QSerialPort::ReadError) {
        qDebug() << QObject::tr("An I/O error occured while reading the data from port %1, error: %2").arg(port->portName()).arg(port->errorString()) << endl;
        QApplication::exit(1);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    port->close();
    qDebug() << "Port closed!!" << endl;
}

void MainWindow::on_btnSend_clicked()
{
    port->write(ui->txtData->text().toStdString().c_str());
}
