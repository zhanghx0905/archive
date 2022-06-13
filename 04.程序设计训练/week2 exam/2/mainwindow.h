#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
#include <QMutex>
#include <QDateTime>
#include <QQueue>
extern int in[100][3],n;

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void valuechanged(int id,int task,int a,int b,bool start);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};


int gcd(int m, int n) ;

struct thread:public QThread{
    Q_OBJECT
   // MainWindow *ui;
public:
    static QQueue<QList<int>> q1,q2;
    static QMutex* m;
    int id;
    void run(){
        switch (id) {
        case 1:{
            int cnt=0;
            while(cnt<n){
                //QMutexLocker mu(m);
                qDebug()<<id<<cnt;
                QList<int> l;
                for(int i=0;i<3;i++)
                    l.push_back(in[cnt][i]);
                emit valuechanged(1,l[0],l[1],l[2],1);
                int c=gcd(l[1],l[2]),d=l[1]*l[2]/c;
                l[1]=c;l[2]=d;
                emit valuechanged(1,l[0],l[1],l[2],0);
                q1.enqueue(l);
                cnt++;
                msleep(1000);
            }
            break;
        }
        case 2:
        {
            int cnt=0;                qDebug()<<id<<cnt;
            while(cnt<n){
                //QMutexLocker mu(m);
                if(q1.empty()){
                    msleep(2000);
                    continue;
                }
                cnt++;
                QList<int> l=q1.dequeue();
                emit valuechanged(2,l[0],l[1],l[2],1);
                int c=l[1]+l[2],d=l[1]*l[2];
                l[1]=c;l[2]=d;
                emit valuechanged(2,l[0],l[1],l[2],0);
                q2.enqueue(l);
                msleep(2000);
            }
            break;
        }
        default:{
            int cnt=0;                qDebug()<<id<<cnt;
            while(cnt<n){
                QMutexLocker mu(m);
                if(q2.empty()){
                    msleep(3000);
                    continue;
                }
                cnt++;
                QList<int> l=q2.dequeue();
                emit valuechanged(3,l[0],l[1],l[2],1);
                int c=l[1]+l[2],d=l[2]-l[1];
                l[1]=c;l[2]=d;
                emit valuechanged(3,l[0],l[1],l[2],0);
                msleep(3000);
            }
        }
        }
    }
signals:
    void valuechanged(int,int,int,int,bool);

};
extern thread t[3];


#endif // MAINWINDOW_H
