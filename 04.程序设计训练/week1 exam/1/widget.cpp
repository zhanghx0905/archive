#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    selected.clear();
    update();
}

void Widget::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    p.setPen(Qt::black);
    int extent=100,a=40,r=15;
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
        {
            rects[i][j].setRect(i*a,j*a,a,a);
            p.setBrush(QBrush(Qt::yellow));
            p.drawRect(rects[i][j]);
            if(selected.contains(QPair<int,int>(i,j)))
                p.setBrush(QBrush(Qt::white));
            else p.setBrush(QBrush(Qt::black));
            p.drawEllipse(rects[i][j].center(),r,r);
        }
}

void Widget::mousePressEvent(QMouseEvent *ev)
{
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++){
            if(rects[i][j].contains(ev->pos())){
                if(selected.contains(QPair<int,int>(i,j))){
                    QString dlgTitle="消息框";
                    QString strInfo="错误！";
                    QMessageBox::warning(this, dlgTitle, strInfo);
                }
                else{
                    selected.insert(QPair<int,int>(i,j));
                    for(int k=0;k<8;k++){
                        if(k!=j){
                            if(selected.contains(QPair<int,int>(i,k)))
                                selected.remove(QPair<int,int>(i,k));
                            else
                                selected.insert(QPair<int,int>(i,k));
                        }
                        if(k!=i){
                            if(selected.contains(QPair<int,int>(k,j)))
                                selected.remove(QPair<int,int>(k,j));
                            else
                                selected.insert(QPair<int,int>(k,j));
                        }
                    }
                }
                break;
            }
        }
    QWidget::mousePressEvent(ev);
    update();
}
