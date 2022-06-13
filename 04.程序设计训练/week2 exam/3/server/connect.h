#ifndef CONNECT_H
#define CONNECT_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QFile>
#include <QDebug>
class Dialog;
class Connect : public QObject
{
    Q_OBJECT

public:
    Connect(qintptr _p,int _id,QObject* p=nullptr);
    QTcpSocket* socket;
    int id;qintptr p;
    QMutex *m;
    QMap<QString,QString> map;
public slots:
    void init();
    void read();
signals:
    void Recv(QString);
};

#endif // CONNECT_H
