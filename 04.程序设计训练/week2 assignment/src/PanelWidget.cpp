#include "PanelWidget.h"
#include "ui_PanelWidget.h"

#include <QDebug>

PanelWidget::PanelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelWidget) {

    ui->setupUi(this);
    ui->lblTurn->setFixedWidth(ui->lcdNumber->width());
    ui->lblColor->setFixedWidth(ui->lcdNumber->width());
    tm = nullptr;
    init();
}

PanelWidget::~PanelWidget() {
    delete ui;
}

void PanelWidget::init() {
    ui->lblTurn->setText("还未开始");
    ui->lblColor->setText("还未开始");
    ui->lcdNumber->display(0);
    sec = 0;
    limit = 30;

    if (tm != nullptr)
        delete tm;
    tm = new QTimer(this);
    tm->stop();
    connect(tm, &QTimer::timeout, [this]{
        --sec;
        ui->lcdNumber->display(sec);
        if (sec == 0)
            emit timeout();
    });
}

void PanelWidget::setColor(bool white)
{
    QString s=tr("你是");
    if (white)
        s += tr("<b>白方</b>");
    else
        s += tr("<b>黑方</b>");
    ui->lblColor->setText(s);
}

void PanelWidget::setTurn(bool white) {
    QString s = tr("轮到");
    if (white)
        s += tr("<b>白方</b>");
    else
        s += tr("<b>黑方</b>");
    ui->lblTurn->setText(s);

    ui->lcdNumber->display(limit);
    sec = limit;
    tm->stop();
    tm->start(1000);
}

void PanelWidget::setTimelimit(int limit) {
    this->limit = limit;
}

int PanelWidget::getTimelimit() const {
    return limit;
}
