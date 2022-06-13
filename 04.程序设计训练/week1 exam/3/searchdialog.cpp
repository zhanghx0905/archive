#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

}

SearchDialog::~SearchDialog()
{
    delete ui;
}

QString SearchDialog::getText(){
    return ui->lineEdit->text();
}
