#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog//, QThread
{
    Q_OBJECT
    QTcpServer *server;
    QTcpSocket *socket;
public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
public slots:
    void onNewConnection();
    void onReadyread();
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
