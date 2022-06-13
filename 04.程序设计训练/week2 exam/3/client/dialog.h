#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "connect.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
public slots:

    void solve(int id);
    void recv(int id, QString str);
private slots:
    void on_btnChoose1_clicked();

    void on_btnChoose2_clicked();

    void on_btnChoose3_clicked();

    void on_btnStart1_clicked();

    void on_btnStart2_clicked();

    void on_btnStart3_clicked();

private:
    Ui::Dialog *ui;
    QThread* thread[3];
    Connect* con[3];
    int cnt=0;
};

#endif // DIALOG_H
