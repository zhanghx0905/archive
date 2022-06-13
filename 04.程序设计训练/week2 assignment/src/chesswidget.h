#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QTextStream>
#include <QTextCodec>

#include "chess.h"
#include "promotion.h"

namespace Ui {
class ChessWidget;
}
 enum State{WIN,LOSE,DRAW,UNKNOWN};
class ChessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChessWidget(QWidget *parent = nullptr);
    ~ChessWidget();
    void init();
    void loadEndgameFromFile(const QString& filename);
    void loadEndgame(QString& str);
    void saveEndgameToFile(const QString& filename);
    QString saveEndgame();
    void setColor(bool white);
    void setTurnColor(bool white);
    void startGame();
    void movePiece(int x1,int y1,int x2,int y2,bool self=true);
    bool isWhite();
    bool isWhiteTurn();
    State getState();
    void setEnd(bool net);
    void setState(State s);
    void promotion(int x,int y,Chess::Type t);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
    void signalEnd(bool);//参数:是否发送网络信号
    void signalTurn(bool);//参数:轮到的是否是白
    void signalMove(int, int, int, int);
    void signalPromote(int,int,Chess::Type);//参数(x,y)，升变成的新棋子
private:
    Ui::ChessWidget *ui;
    inline double posX(int x);
    inline double posY(int y);
    void turn(); // 交换行棋权

    Chess::Color color; // 用户所在方颜色
    Chess::Color turnColor; // 行棋方颜色
    int selX, selY; // 鼠标选中棋子坐标
    QRect rects[8][8];
    bool selShow; // 选中的棋子是否显示；否则不显示
    QTimer *tm; //用于实现棋子闪烁
    State state; // 战斗状态
    bool end; //当前是否在战斗

    Promotion* promotionDialog;
};
#endif // CHESSWIDGET_H
