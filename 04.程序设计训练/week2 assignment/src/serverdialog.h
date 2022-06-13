#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>

#include "loadingdialog.h"

namespace Ui {
class ServerDialog;
}

class ServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerDialog(QWidget *parent = nullptr);
    ~ServerDialog();

    QTcpSocket *getSocket();
    void setTurn(const QString&trun);
    int getTurn();//0白，1黑
    int getSecond();
public slots:
    void accept();//重写accept
private:
    Ui::ServerDialog *ui;
    LoadingDialog* loading;
    QTcpServer *server;
    QTcpSocket* socket;
};

#endif // SERVERDIALOG_H
