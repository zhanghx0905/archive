#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QObject>
#include <QTextCodec>
#include <QPainter>
#include <QThread>
#include <QHostAddress>
#include <QMutex>
#include <QTcpSocket>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog//,public QThread
{
    Q_OBJECT
    QTcpSocket* socket;
public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void onReadyread();
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
