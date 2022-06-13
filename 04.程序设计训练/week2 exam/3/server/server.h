#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QTcpServer>
#include <QThread>
#include "connect.h"
class Server : public QTcpServer
{
    //Q_OBJECT
public:

    Server(QObject *p=nullptr);
public slots:
    void incomingConnection(qintptr handle) override;
    void Read(QString str);
private:
    QThread* thread[1000];
    Connect* con[1000];
    int cnt=0;

signals:
    //void Recvr(QString);
};

#endif // SERVER_H
