#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::solve(int id)
{
    QString FileName = QFileDialog::getOpenFileName(this,"Open",QDir::currentPath(),"all(*.*)");
    thread[id]=new QThread(this);
    con[id]= new Connect(id,FileName);
    con[id]->moveToThread(thread[id]);
    connect(thread[id],SIGNAL(started()),con[id],SLOT(init()));
    connect(con[id],&Connect::Recv,this,&Dialog::recv);

}

void Dialog::recv(int id,QString str)
{
    if(id==0){
        ui->btnStart1->setEnabled(1);
        ui->textEdit1->setText(str);
        thread[0]->quit();
    }
    else if(id==1){
         ui->textEdit2->setText(str);
               ui->btnStart2->setEnabled(1);
                 thread[1]->quit();
    }
    else {
         ui->textEdit3->setText(str);
              ui->btnStart3->setEnabled(1);
                thread[2]->quit();
    }
}

void Dialog::on_btnChoose1_clicked()
{
    solve(0);
}

void Dialog::on_btnChoose2_clicked()
{
    solve(1);
}

void Dialog::on_btnChoose3_clicked()
{
    solve(2);
}

void Dialog::on_btnStart1_clicked()
{ui->textEdit1->clear();
    ui->btnStart1->setDisabled(1);
    if(thread[0]){
        thread[0]->start();
    }
}

void Dialog::on_btnStart2_clicked()
{ui->textEdit2->clear();
    ui->btnStart2->setDisabled(1);
    if(thread[1])
        thread[1]->start();
}

void Dialog::on_btnStart3_clicked()
{
    ui->textEdit3->clear();
        ui->btnStart3->setDisabled(1);
    if(thread[2])
        thread[2]->start();
}
