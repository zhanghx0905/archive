#include "server.h"
Server::Server(QObject *p)
    :QTcpServer (p){
}

void Server::incomingConnection(qintptr handle)
{

    qDebug()<<cnt<<"connecting";
    con[cnt]=new Connect(handle,cnt);
    thread[cnt]=new QThread;
    con[cnt]->moveToThread(thread[cnt]);
    connect(thread[cnt],&QThread::started,con[cnt],&Connect::init);
    //connect(con[cnt],&Connect::Recv,this,&Server::Recvr);
    thread[cnt]->start();
    cnt++;
}
/*
void Server::Read(QString str)
{
    emit Recvr(str);
}*/
