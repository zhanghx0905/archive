#ifndef PANELWIDGET_H
#define PANELWIDGET_H

#include <vector>
#include <QWidget>
#include <QTimer>

#include "chess.h"
namespace Ui {
class PanelWidget;
}
class PanelWidget : public QWidget {
    Q_OBJECT

public:
    explicit PanelWidget(QWidget *parent = 0);
    ~PanelWidget();

    void init();

    void setColor(bool white);
    void setTimelimit(int limit);
    int getTimelimit() const;
public slots:
    void setTurn(bool white);//若为true，表示当前下棋的是白方；否则是黑方
signals:
    void timeout();//可用时间为0

private:

    Ui::PanelWidget *ui;

    QTimer *tm;
    int sec; // 该回合剩的秒数
    int limit; // 回合上限时间
};

#endif // PANELWIDGET_H
