#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString curPath=QDir::currentPath();
    QString digTitle="open a document.";
    QString filter="txt(*.txt);;all(*.*)";
    QString aFileName=QFileDialog::getOpenFileName(this,digTitle,curPath,filter);
    if(!aFileName.isEmpty())
        openText(aFileName);
}

void MainWindow::openText(QString Filename)
{
    QFile f(Filename);
    if(!f.exists()||!f.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    QTextStream stream(&f);
//point: 读取中文
    stream.setCodec(QTextCodec::codecForName("UTF-8"));
    ui->textEdit->setText(stream.readAll());
    f.close();
    mark=true;
    filename=Filename;
}



void MainWindow::on_actionNew_triggered()
{
    ui->textEdit->clear();mark=false;
}

void MainWindow::on_actionSave_triggered()
{
    if(!mark){
        QString curPath=QDir::currentPath();
        QString digTitle="Save";
        QString filter="txt(*.txt)";
        filename=QFileDialog::getSaveFileName(this,digTitle,curPath,filter);
    }
    QFile   aFile(filename);
    if(filename.isEmpty()) return;
    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QString str=ui->textEdit->toPlainText();//整个内容作为字符串

    QByteArray  strBytes=str.toUtf8();//转换为字节数组
    aFile.write(strBytes,strBytes.length());  //写入文件

    aFile.close();

}
