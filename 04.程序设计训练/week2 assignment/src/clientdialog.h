#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QRegExp>

#include "loadingdialog.h"
namespace Ui {
class ClientDialog;
}

class ClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientDialog(QWidget *parent = nullptr);
    ~ClientDialog();
    QTcpSocket* getSocket();
private slots:
    void accept();
private:
    Ui::ClientDialog *ui;
    LoadingDialog* loading;
    QTcpSocket *socket;
};

#endif // CLIENTDIALOG_H
