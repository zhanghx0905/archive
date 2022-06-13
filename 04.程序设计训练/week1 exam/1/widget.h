#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTextStream>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QObject>
#include <QTextCodec>
#include <QPainter>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    QRect rects[8][8];

    QSet<QPair<int,int>> selected;
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    void paintEvent(QPaintEvent* ev);
    void mousePressEvent ( QMouseEvent * ev );
};

#endif // WIDGET_H
