#ifndef CONNECT_H
#define CONNECT_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QHostAddress>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QObject>
#include <QTextCodec>
#include <QPainter>
#include <QThread>
#include <QHostAddress>
#include <QMutex>

class Connect : public QThread
{
    Q_OBJECT
    int id;QString fileName;
    QTcpSocket* socket;
    QMutex* mutex;
public:
    Connect(int _id,QString f,QObject *parent = nullptr);

signals:
    void Recv(int id,QString str);
public slots:
    void init();
    void read();
    void send();
};

#endif // CONNECT_H
