#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QHostAddress>
#include "server.h"

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
    void changed(QString str);
private:
    Ui::Dialog *ui;
    Server *server;
};

#endif // DIALOG_H
