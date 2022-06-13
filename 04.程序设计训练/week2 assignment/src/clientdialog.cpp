#include "clientdialog.h"
#include "ui_clientdialog.h"

ClientDialog::ClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientDialog)
{
    ui->setupUi(this);
    ui->editPort->setValidator(new QIntValidator(0, 65535, this));
    ui->editPort->setText("8080");

    QRegExp reg = QRegExp("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
    ui->editIP->setValidator(new QRegExpValidator(reg, this));
    loading = new LoadingDialog(tr(u8"连接到对战"), this);
    socket = nullptr;

    connect(loading, &LoadingDialog::rejected, [this]{
        socket->abort();
    });
}

ClientDialog::~ClientDialog()
{
    delete ui;
}

QTcpSocket *ClientDialog::getSocket(){
    return socket;
}

void ClientDialog::accept()
{
    auto ip = ui->editIP->text();
    int port = ui->editPort->text().toInt();

    loading->setText(tr(u8"正在连接%1:%2...").arg(ip).arg(port));
    loading->startGif();
    loading->show();
    if (socket != nullptr)
        delete socket;
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, [this]{
        loading->stopGif();
        loading->hide();
        QDialog::accept();
    });
    socket->connectToHost(ip, port);
}
