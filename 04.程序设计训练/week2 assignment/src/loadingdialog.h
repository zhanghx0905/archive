#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>

namespace Ui {
class LoadingDialog;
}

class LoadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingDialog(const QString &title,
                           QWidget *parent = nullptr);
    ~LoadingDialog();

    void setText(const QString &text);
    void startGif();
    void stopGif();

private:
    Ui::LoadingDialog *ui;
    QMovie *gif;
};

#endif // LOADINGDIALOG_H
