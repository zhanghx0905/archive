#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

#include "chesswidget.h"
#include "PanelWidget.h"
#include "clientdialog.h"
#include "serverdialog.h"

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
    void on_actionInit_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionQuit_triggered();

    void on_actionServer_triggered();
    void on_actionAD_triggered();
    void on_actionClient_triggered();

    void socketRead();
    void socketWriteInit();
    void socketWriteMove(int x1, int y1, int x2, int y2);
    void socketWriteLose();
    void socketWriteWin();
    void socketWriteDraw();
    void socketWritePromote(int x,int y,Chess::Type t);


private:
    Ui::MainWindow *ui;
    ChessWidget *w; //主部件

    QTcpSocket *socket; //用于通信的socket

    ServerDialog *serverDialog;
    ClientDialog *clientDialog;

    PanelWidget *panelWidget;
    QDockWidget *dockWidget; //包含panelWidget的dock
};

#endif // MAINWINDOW_H
