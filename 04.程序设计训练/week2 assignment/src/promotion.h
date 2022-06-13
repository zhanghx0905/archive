#ifndef PROMOTION_H
#define PROMOTION_H

#include <QDialog>
#include "chess.h"
namespace Ui {
class Promotion;
}

class Promotion : public QDialog
{
    Q_OBJECT

public:
    explicit Promotion(QWidget *parent = nullptr);
    ~Promotion();
    Chess::Type getType();
private:
    Ui::Promotion *ui;
};

#endif // PROMOTION_H
