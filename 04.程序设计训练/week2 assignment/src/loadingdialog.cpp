#include "loadingdialog.h"
#include "ui_loadingdialog.h"
#include <QMovie>
LoadingDialog::LoadingDialog(const QString &title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);

    gif = new QMovie(":/res/res/loading.gif");
    ui->labelMovie->setMovie(gif);
}

LoadingDialog::~LoadingDialog()
{
    delete gif;
    delete ui;
}

void LoadingDialog::setText(const QString &text)
{
    ui->labelText->setText(text);
}

void LoadingDialog::startGif()
{
    gif->start();
}

void LoadingDialog::stopGif()
{
    gif->stop();
}
