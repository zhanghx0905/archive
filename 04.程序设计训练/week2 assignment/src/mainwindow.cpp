#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setFixedSize(width(),height());
    w = new ChessWidget(this);
    setCentralWidget(w);

    serverDialog = new ServerDialog(this);
    serverDialog->setTurn(w->isWhiteTurn() ? u8"白方" : u8"黑方");

    clientDialog = new ClientDialog(this);

    socket = nullptr;

    panelWidget = new PanelWidget(this);
    ui->actionAD->setEnabled(false);
    dockWidget = new QDockWidget(this);
    dockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    dockWidget->setWidget(panelWidget);
    dockWidget->setTitleBarWidget(new QWidget(dockWidget));//
    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);

    connect(panelWidget, &PanelWidget::timeout, [this]{
        if (w->isWhiteTurn() == w->isWhite()) {
            w->setState(State::LOSE);//先setState再setEnd
            w->setEnd(true);
        }
    });
    connect(w, &ChessWidget::signalTurn, panelWidget, &PanelWidget::setTurn);
    connect(w, &ChessWidget::signalEnd, [this](bool net){
        State state=w->getState();
        panelWidget->init();
        ui->actionAD->setEnabled(false);
        //qDebug()<<"signalEnd";
        if (state==State::WIN) {
            if(net) socketWriteWin();
            QMessageBox::information(this, tr(u8"对战结束"), tr(u8"获胜"));    
        }
        else if(state==State::LOSE){
            if(net) socketWriteLose();
            QMessageBox::information(this, tr(u8"对战结束"),tr(u8"失败"));        
        }
        else if(state==State::DRAW){
            if(net) socketWriteDraw();
            QMessageBox::information(this, tr(u8"对战结束"),tr(u8"平局"));  
        }
        socket = nullptr;
    });
    connect(w, &ChessWidget::signalMove, this, &MainWindow::socketWriteMove);
    connect(w,&ChessWidget::signalPromote,this,&MainWindow::socketWritePromote);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionInit_triggered()
{
    if (socket != nullptr) {
        QMessageBox::warning(this,tr(u8"错误"),tr(u8"正在对战，无法初始化局面"));
        return;
    }
    w->init();
}

void MainWindow::on_actionOpen_triggered()
{
    if (socket != nullptr) {
        QMessageBox::warning(this,tr(u8"错误"),tr(u8"正在对战，无法加载残局"));
        return;
    }
    QString curPath=QDir::currentPath();
    auto filename = QFileDialog::getOpenFileName(this,tr(u8"打开残局"),
                                                 curPath,tr(u8"txt(*.txt)"));
    if (!filename.isEmpty())
        w->loadEndgameFromFile(filename);
    //if(socket!=nullptr)
       // socketWriteInit();
}

void MainWindow::on_actionSave_triggered()
{
    QString curPath=QDir::currentPath();
    auto filename = QFileDialog::getSaveFileName(this,tr(u8"保存当前局面"),
                                                 curPath, tr(u8"文本文件 (*.txt)"));
    if (!filename.isEmpty())
        w->saveEndgameToFile(filename);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionServer_triggered()
{
    if (serverDialog->exec() == QDialog::Accepted) {
        socket = serverDialog->getSocket();
        w->setColor(serverDialog->getTurn() == 0);//getTurn 0 白 1黑
        panelWidget->setTimelimit(serverDialog->getSecond());
        ui->actionAD->setEnabled(true);
        panelWidget->setColor(serverDialog->getTurn() == 0);
        w->startGame();
        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketRead);
        socketWriteInit();
        QMessageBox::information(this,tr(u8"对战"),
                                 tr(u8"连接成功！你将与IP为%1的用户对战").arg(socket->peerAddress().toString()));
    }
}

void MainWindow::on_actionClient_triggered()
{
    if (clientDialog->exec() == QDialog::Accepted) {
        socket = clientDialog->getSocket();
        ui->actionAD->setEnabled(true);
        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketRead);
        QMessageBox::information(this,tr(u8"对战"),
                                 tr(u8"连接成功！将与IP为%1的用户对战").arg(socket->peerAddress().toString()));
    }
}

void MainWindow::socketRead()
{
    auto data = socket->readAll();
    QDataStream st(&data, QIODevice::ReadOnly);
    while (!st.atEnd()) {
        int op;
        st >> op;
        if (op == 0){
            int color,limit;
            QString endgame;
            st >> color>> limit>> endgame;

            w->setColor(color == 0);
            panelWidget->setTimelimit(limit);
            panelWidget->setColor(color==0);
            w->loadEndgame(endgame);
            w->startGame();
            return;
        }
        else if (op == 1){
            //qDebug()<<"signalmove";
            int x1, y1, x2, y2;
            st >> x1 >> y1 >> x2 >> y2;
            w->movePiece(x1, y1, x2, y2, false);
        }
        else if(3<=op&&op<=5){
            if (op == 3)
                w->setState(State::WIN);
            else if (op == 4)
                w->setState(State::LOSE);
            else if (op==5)
                w->setState(State::DRAW);
            else return;
            w->setEnd(false);
            panelWidget->init();
        }
        else if(op==2){
            int x,y,tmp;
            st>>x>>y>>tmp;
            w->promotion(x,y,(Chess::Type)tmp);
        }

    }
}

void MainWindow::socketWriteInit()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << 0;
    stream << int(w->isWhite());
    stream << panelWidget->getTimelimit();
    stream << w->saveEndgame();
    socket->write(data);
    socket->waitForBytesWritten();
}

void MainWindow::socketWriteMove(int x1, int y1, int x2, int y2) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << 1 << x1 << y1 << x2 << y2;
    socket->write(data);
    socket->waitForBytesWritten();
}


void MainWindow::socketWriteLose() {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << 3;
    socket->write(data);
    socket->waitForBytesWritten();
}

void MainWindow::socketWriteWin() {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << 4;
    socket->write(data);
    socket->waitForBytesWritten();
}

void MainWindow::socketWriteDraw()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << 5;
    socket->write(data);
    socket->waitForBytesWritten();
}

void MainWindow::socketWritePromote(int x, int y, Chess::Type t)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << 2<<x<<y<<t;
    socket->write(data);
    socket->waitForBytesWritten();
}

void MainWindow::on_actionAD_triggered()
{
    auto ret = QMessageBox::warning(this,
                                    tr(u8"认输"),
                                    tr(u8"确定认输？"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        w->setState(State::LOSE);
        w->setEnd(true);
    }
}
