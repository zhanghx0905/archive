#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    server=new Server(this);
    server->listen(QHostAddress::LocalHost,2018);
    //connect(server,&Server::Recvr,this,&Dialog::changed);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::changed(QString str)
{
    ui->textEdit->insertPlainText(str);
}
