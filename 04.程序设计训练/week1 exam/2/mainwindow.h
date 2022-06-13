#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTextStream>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QObject>
#include <QTextCodec>
#include <QPainter>
#include <QGraphicsRectItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int iter;
    QTimer* tm;
    QList<QPixmap> pic;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnBiscuit_clicked();
    void on_timeout();
    void on_btnSteak_clicked();

    void on_btnNoodle_clicked();

    void on_btnCola_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
