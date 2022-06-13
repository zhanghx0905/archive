#include "qwgraphicsview.h"
#include    <QMouseEvent>
#include    <QPoint>

void QWGraphicsView::mousePressEvent(QMouseEvent *event)
{ //鼠标左键按下事件
    if (event->button()==Qt::LeftButton)
    {
        QPoint point=event->pos(); //QGraphicsView的坐标
        emit mouseClicked(point);//释放信号
    }
    QGraphicsView::mousePressEvent(event);
}


void QWGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{ //鼠标双击事件
    if (event->button()==Qt::LeftButton)
    {
        QPoint point=event->pos(); //QGraphicsView的坐标
        emit mouseDoubleClick(point);//释放信号
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

void QWGraphicsView::keyPressEvent(QKeyEvent *event)
{
    emit keyPress(event);
    QGraphicsView::keyPressEvent(event);
}


QWGraphicsView::QWGraphicsView(QWidget *parent):QGraphicsView(parent)
{}
