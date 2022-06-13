#include "connect.h"

Connect::Connect(qintptr _p, int _id, QObject* pa)
    :QObject(pa),id(_id),p(_p)
{

}

void Connect::init(){
    socket = new QTcpSocket;
    socket->setSocketDescriptor(p);
    m=new QMutex;
    connect(socket,&QTcpSocket::readyRead,this,&Connect::read);
}

void Connect::read(){
    QMutexLocker lock(m);
    QString out;
    while(socket->canReadLine()){
         QString in=socket->readLine();
         qDebug()<<in;        emit Recv(in);
         in.replace("\r\n","");
         QStringList l=in.split(" ");
         if(l[0]=="put"){
             map[l[1]]=l[2];
         }
         else if(l[0]=="get"){
             if(map.contains(l[1])&&!map[l[1]].isEmpty())
                 out+=map[l[1]]+"\n";
             else out+="null\n";
         }
    }
    socket->write(out.toUtf8());

}
