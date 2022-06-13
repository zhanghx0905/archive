#include "serverdialog.h"
#include "ui_serverdialog.h"
#include <QtNetwork>

ServerDialog::ServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDialog)
{
    ui->setupUi(this);

    ui->editPort->setValidator(new QIntValidator(0, 65535, this));
    ui->editPort->setText("8080");

    ui->editTime->setValidator(new QIntValidator(1, 180, this));
    ui->editTime->setText("60");

    loading = new LoadingDialog(tr("建立服务器"), this);
    socket = nullptr;

    //获取本机地址
    QHostInfo   hostInfo=QHostInfo::fromName(QHostInfo::localHostName());

    QList<QHostAddress> addList=hostInfo.addresses();//
    if (!addList.isEmpty())
        for (int i=0;i<addList.count();i++){
            QHostAddress aHost=addList.at(i);
            if (QAbstractSocket::IPv4Protocol==aHost.protocol()){
                QString IP=aHost.toString();
                if (ui->cbbBoxIP->findText(IP)<0)
                    ui->cbbBoxIP->addItem(IP);
            }
        }

    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, [this]{
        if (socket != nullptr)
            delete socket;
        socket = server->nextPendingConnection();
        server->close();
        loading->stopGif();
        loading->hide();
        QDialog::accept();
    });
    connect(loading, &LoadingDialog::rejected, [this]{
        server->close();
    });
}



ServerDialog::~ServerDialog()
{
    delete ui;
}

QTcpSocket *ServerDialog::getSocket()
{
    return socket;
}

void ServerDialog::setTurn(const QString &turn)
{
    ui->lblTurn->setText(ui->lblTurn->text()+" "+turn);
}

int ServerDialog::getTurn()
{
    return ui->cbboxPlay->currentIndex();
}

int ServerDialog::getSecond()
{
    return ui->editTime->text().toInt();
}

void ServerDialog::accept()
{
    if (server->isListening())
        server->close();
    int port = ui->editPort->text().toInt();
    QString ip=ui->cbbBoxIP->currentText();
    loading->setText(tr(u8"正在监听%1:%2\n等待连接中...").arg(ip).arg(port));
    loading->startGif();
    loading->show();
    server->listen(QHostAddress::AnyIPv4,port);
}
