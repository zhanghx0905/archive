#include "connect.h"


Connect::Connect(int _id, QString f, QObject *parent)
    :QThread (parent),id(_id),fileName(f)
{}

void Connect::init(){
    if(socket) socket=nullptr;
    socket = new QTcpSocket;
    socket->connectToHost(QHostAddress::LocalHost,2018);
    mutex = new QMutex;
    connect(socket,&QTcpSocket::readyRead,this,&Connect::read);
    send();
}

void Connect::read()
{
    QString tmp=socket->readAll();
    emit Recv(id,tmp);
   // terminate();
}

void Connect::send()
{
    QMutexLocker lock(mutex);
    QFile f(fileName);
    f.open(QFile::ReadOnly);
    socket->write(f.readAll());
    f.close();
    qDebug()<<id<<"send end!";
}
