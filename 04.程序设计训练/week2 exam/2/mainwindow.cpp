#include "mainwindow.h"
#include "ui_mainwindow.h"
int in[100][3],n;
thread t[3];
QQueue<QList<int>> thread::q1;
QQueue<QList<int>> thread::q2;
QMutex* thread::m=new QMutex;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile aFile("input.txt");
    if (!aFile.exists()||!aFile.open(QIODevice::ReadOnly))
        return;
    QTextStream s(&aFile);
    QString input;QStringList l;
    while(!s.atEnd()){
        input=s.readLine();input.replace("\n","");
        l=input.split(" ");
        for(int i=0;i<l.size();i++)
            in[n][i]=l[i].toInt();
        n++;
    }
    aFile.close();
    qDebug()<<n;
    for(int i=0;i<3;i++){

        t[i].id=i+1;
        connect(&t[i],&thread::valuechanged,this,&MainWindow::valuechanged);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    for(int i=0;i<3;i++)
        t[i].start();
}

void MainWindow::valuechanged(int id, int task, int a, int b, bool start)
{
    QString out=QDateTime::currentDateTime().toString("hh:mm:ss.zzz")+" Task "+
            QString::number(task);
    if(start)
        out+=" start ";
        else {
            out+=" complete ";
        }
    out+=QString::number(a)+" "
            +QString::number(b)+"\n";
    if(id==1){
        ui->textEdit->insertPlainText(out);
    }
    else if(id==2){
        ui->textEdit_2->insertPlainText(out);
    }
    else {
        ui->textEdit_3->insertPlainText(out);
    }
}

int gcd(int m, int n)
{
        while (m > 0 && n > 0) {
            if (m >= n)
                m = m % n;
            else
                n = n % m;
        }
        return m + n;

}
