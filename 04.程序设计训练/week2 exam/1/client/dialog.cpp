#include "dialog.h"
#include "ui_dialog.h"
#include <QHostAddress>
#include <QThread>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    socket = new QTcpSocket;
    socket->connectToHost(QHostAddress::LocalHost,8080);
    connect(socket,&QTcpSocket::readyRead,this,&Dialog::onReadyread);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QByteArray str;

    QDataStream s(&str,QIODevice::ReadWrite);
    QString tmp=ui->editYuan->text();
    //s.writeBytes()
    //源路径
    qDebug()<<ui->editYuan->text();
    s<<ui->editYuan->text();
    socket->write(tmp.toLatin1());
}

void Dialog::onReadyread(){
    QDataStream s(socket);
    QByteArray out;
    qint64 size,cur=0;
    s>>size;
    qDebug()<<size;
    QString FileName=ui->editXian->text();
    QFile aFile(FileName+"/1.mp3");
    qDebug()<<aFile.open(QIODevice::ReadWrite);
    QDataStream st(&aFile);
    while (cur < size){
        ui->textEdit->clear();
        socket->waitForReadyRead(150);
        QByteArray arr = socket->readAll();
        cur += arr.size();
        qDebug()<<"read"<<cur;
        st<<arr;
        ui->textEdit->setText(tr("已接收%1KB，共计%2MB").arg(cur/1024).arg(size/(1024*1024)));

    }

    qDebug()<<"end";
    aFile.close();
    return;

}
