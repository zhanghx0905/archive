#include "promotion.h"
#include "ui_promotion.h"

Promotion::Promotion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Promotion)
{
    ui->setupUi(this);
}

Promotion::~Promotion()
{
    delete ui;
}

Chess::Type Promotion::getType()
{
    QString str=ui->comboBox->currentText();
    Chess::Type ans;
    if(str=="皇后")
        ans=Chess::QUEEN;
    else if(str=="车")
        ans=Chess::ROOK;
    else if(str=="马")
        ans=Chess::KNIGHT;
    else ans=Chess::BISHOP;

    return ans;
}
