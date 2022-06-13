#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSound>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tm=new QTimer;
    tm->setInterval(2000);
    tm->stop();
    connect(tm,&QTimer::timeout,this,&MainWindow::on_timeout);

    QImage* biscuit=new QImage(":/pic/2/biscuit.jpg"),
            *steak=new QImage(":/pic/2/steak.jpg"),
            *noodle=new QImage(":/pic/2/noodle.jpg"),
            *cola=new QImage(":/pic/2/cola.jpg");
    pic.push_back(QPixmap(QPixmap::fromImage(*biscuit)));
    pic.push_back(QPixmap(QPixmap::fromImage(*steak)));
    pic.push_back(QPixmap(QPixmap::fromImage(*noodle)));
    pic.push_back(QPixmap(QPixmap::fromImage(*cola)));
    iter=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBiscuit_clicked()
{
    iter=0;
    tm->stop();
    on_timeout();
    tm->start();
}

void MainWindow::on_timeout()
{

    if(iter>=4) iter=0;
    QPixmap dest=pic[iter].scaled(ui->labelPic->size(),Qt::KeepAspectRatio);
    ui->labelPic->setPixmap(dest);
    switch (iter) {
    case 0:
        QSound::play(":/pic/2/biscuit.wav");
        break;
    case 1:
        QSound::play(":/pic/2/steak.wav");
        break;
    case 2:
        QSound::play(":/pic/2/noodle.wav");
        break;
    case 3:
        QSound::play(":/pic/2/cola.wav");
        break;
    }
        iter++;
}

void MainWindow::on_btnSteak_clicked()
{
    iter=1;
    tm->stop();
    on_timeout();
    tm->start();
}

void MainWindow::on_btnNoodle_clicked()
{
    iter=2;
    tm->stop();
    on_timeout();
    tm->start();
}

void MainWindow::on_btnCola_clicked()
{
    iter=3;
    tm->stop();
    on_timeout();
    tm->start();
}
