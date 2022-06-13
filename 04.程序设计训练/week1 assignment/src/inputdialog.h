#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QPair>
#include <QVector>
#include <QMessageBox>
#include <QLayout>
#include <QSpinBox>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();

    QPair<int,int> outputPos();
    int rowNum();
    int colNum();
    bool wash();
    QSet<QPair<int,int> > inputPos();
private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_sBoxRow_valueChanged(int arg1);

    void on_sBoxCol_valueChanged(int arg1);

    void on_cBoxWash_clicked(bool checked);

private:
    QVector<QSpinBox *> sBoxInput;
    Ui::InputDialog *ui;

};

#endif // INPUTDIALOG_H
