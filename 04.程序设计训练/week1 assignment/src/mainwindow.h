#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QTimer>
#include <QMap>
#include <QLabel>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QLCDNumber>
#include <QtMultimedia/QSound>

#include "inputdialog.h"
#include "qwgraphicsview.h"
#include "solver.h"

typedef QPair<int,int> Corrd;

struct Liquid{//液滴
    QSet<QPair<int,int>> vis;//已经访问过的点
    QGraphicsEllipseItem* item;
    Liquid():item(nullptr){}
    Liquid(const Liquid& obj)=delete ;
};

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum Op{ Input,Move,Split,Merge,Output };
    enum class State{ Begin,Play,End,Wash };
    enum class MusicOp{ Null,Move,Spliting,Splited,Merge};

    //GraphicsItem信息项
    static const int rowId=1;//行
    static const int colId=2;//列
    static const int InfoId=3;//种类
    static const int NoId=4;//液滴编号

    static const int r=25;//液滴的标准半径
    static const int width=80,height=50;//方块宽高
    static const int extent=50; //偏移量

    QGraphicsScene  *scene;
    QLabel  *labItemInfo;
    QLCDNumber *lcdTime;
    QTimer* tm;

    int row,col;//行列数
    int seqNum;//液滴编号
    Corrd outPos;//输出端位置
    QSet<Corrd> inPosSet;//输入端位置
    bool Wash;//是否开启清洗
    QVector<QVector<QPointF>> rectPos;//矩形的位置坐标
    QVector<QVector<QGraphicsRectItem*>> rect;//矩形控件

    //每次重设行列都要清空或清0
    QMap<int,QMap<Corrd,Liquid*>> liquid;//t时刻图像上(x,y)处的液滴
    QMultiMap<int,QVector<int>> opMap;//t时刻的操作集合
    QMultiMap<int,MusicOp> opMusic;//t时刻播放的音乐种类
    QMap<int,QMap<Corrd,QSet<int>>> rectDirt;//t时刻(x,y)位置处矩阵的污染种类

    QSet<Corrd> UnWashable;//不能被清洗的格子集合
    QMultiMap<int,QPair<int,Corrd>> toWash; //需要在[l,r]内完成清洗的坐标
    QSet<QPair<int,Corrd>> Washing;//正在寻求清洗的坐标

    Corrd target;//清洗目标的位置
    QGraphicsEllipseItem* washDrip=nullptr;//清洗液滴
    Corrd washDripPos;//清洗液滴的位置
    bool washed;//预定点是否已经被洗到

    int xAround[9]={-1,-1,-1,0,0,0,1,1,1};
    int yAround[9]={-1,0,1,-1,0,1,-1,0,1};
    int t,Maxt;
    bool Error;//标记是否满足移动约束
    State state;


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionIni_triggered();
    void on_actionopen_triggered();
    void on_actionExpand_triggered();
    void on_actionShrink_triggered();
    void on_actionNext_triggered();
    void on_actionBack_triggered();
    void on_actionReset_triggered();
    void on_actionPlay_triggered();
    void on_timer_timeout();
    void on_actionStop_triggered();
    void on_timeChanged(int time);
    void on_actionRotateRight_triggered();
    void on_actionRotateLeft_triggered();
    void on_actionText_triggered();

    void on_mouseClicked(QPoint point);
    void on_mouseDoubleClick(QPoint point);
    void on_keyPress(QKeyEvent *event);
signals:
    void time_changed(int);
private:
    Ui::MainWindow *ui;
    void viewInit();
    void openText(QString Filename);    
    void preProcess();
    void washPaint(QList<Corrd> route);
    void playHandle(int flag);//flag=1,向前,flag=-1,向后
    void washHandle();
    void checkWash(int time);
    void playMusic(int time);
    bool stateValid(int time);
    Liquid* findObject(int rank,Corrd corrd);
};

#endif // MAINWINDOW_H
