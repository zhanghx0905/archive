#include "dialog.h"
#include "ui_dialog.h"
#include <QFile>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    server=new QTcpServer;
    connect(server,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    quint16 port=8080;//端口
    server->listen(QHostAddress::LocalHost,port);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onNewConnection()
{

    socket = server->nextPendingConnection();
    connect(socket,&QTcpSocket::readyRead,this,&Dialog::onReadyread);

}

void Dialog::onReadyread()
{
    QString fileName=socket->readAll();
    qDebug()<<fileName;
    ui->textEdit->setText(fileName);
    QFile f(fileName);
     f.open(QFile::ReadWrite);

     QByteArray arr;
     QDataStream out(&arr,QIODevice::ReadWrite);
     qint64 size=f.size();
     out<<size;
     socket->write(arr);
     socket->waitForBytesWritten();
     for (int cur=0;cur<size;) {
         QByteArray block=f.read(40960);
         cur += block.size();
         qDebug()<<"cur"<<cur;
         socket->write(block);
         socket->waitForBytesWritten(100);
                 //msleep(100);
     }
    qDebug()<<"end";
     f.close();
}
