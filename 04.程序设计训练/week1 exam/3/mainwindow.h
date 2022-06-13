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
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();


    void on_actionNew_triggered();

    void on_actionSave_triggered();

private:
    void openText(QString Filename);
    int SearchText(QString pat,QString raw);
    QString filename;bool mark=false;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
