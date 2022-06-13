#include "chesswidget.h"
#include "ui_chesswidget.h"
#include <QDir>
#include <QDebug>

ChessWidget::ChessWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChessWidget)
{
    ui->setupUi(this);
    promotionDialog=new Promotion;
    tm = new QTimer(this);
    tm->setInterval(300);tm->stop();
    this->setMouseTracking(true);
    connect(tm, &QTimer::timeout, [this]{
        selShow = !selShow;
        this->repaint();
    });
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++){
            rects[i][j].setRect(38+94.7*i, 46+94.7*j,94,94);
        }
    setFixedSize(839, 861);//与棋盘图片大小相同
    init();
}

ChessWidget::~ChessWidget()
{
    delete ui;
}

void ChessWidget::init()
{
    Chess::init();
    selX = -1;
    selY = -1;
    color = Chess::WHITE;
    turnColor = Chess::WHITE;
    selShow = false;
    state = State::UNKNOWN;
    end = true;
    repaint();
}

void ChessWidget::loadEndgameFromFile(const QString &filename)
{
    QFile f(filename);
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream st(&f);
    st.setCodec(QTextCodec::codecForName("GB2312"));
    QString tmp=st.readAll();
    loadEndgame(tmp);
    st.flush();
    f.close();
}

void ChessWidget::loadEndgame(QString& str)
{
    Chess::clear();
    //qDebug()<<str;
    QTextStream st(&str);
    Chess::Type tmp_type;Chess::Color tmp_color;

    QString tmp=st.readLine().replace("\n","");
    if(tmp=="white")
        tmp_color=turnColor=Chess::Color::WHITE;
    else if(tmp=="black")
        tmp_color=turnColor=Chess::Color::BLACK;
    else return;
    //qDebug()<<tmp;
    while(!st.atEnd()){
        tmp=st.readLine().replace("\n","");
        if(tmp=="black"){
            tmp_color=Chess::Color::BLACK;
            continue;
        }
        else if(tmp=="white") {
            tmp_color=Chess::Color::WHITE;
            continue;
        }

        QStringList list=tmp.split(" ");
        if(list[0]=="king")
            tmp_type=Chess::Type::KING;
        else if(list[0]=="queen")
            tmp_type=Chess::Type::QUEEN;
        else if(list[0]=="rook")
            tmp_type=Chess::Type::ROOK;
        else if(list[0]=="bishop")
            tmp_type=Chess::Type::BISHOP;
        else if(list[0]=="pawn")
            tmp_type=Chess::Type::PAWN;
        else if(list[0]=="knight")
            tmp_type=Chess::Type::KNIGHT;
        else return;

        int num=list[1].toInt(),i=2;
        while(num--){
            int x=list[i][0].toLatin1()-'a',
                    y=list[i][1].toLatin1()-'1';
            Chess::setPiece(x,y,tmp_type,tmp_color);
            i++;
         //   qDebug()<<x<<y<<tmp_type<<tmp_color;
        }
    }
    st.flush();
}

void ChessWidget::saveEndgameToFile(const QString &filename)
{
    QFile f(filename);
    f.open(QFile::WriteOnly | QFile::Text);
    QTextStream st(&f);
    st << saveEndgame();
    st.flush();
    f.close();
}

QString ChessWidget::saveEndgame()
{
    vector<Pos> pos[2][6];
    QString color[2],type[6];
    color[Chess::WHITE] = "white"; color[Chess::BLACK] = "black";
    type[Chess::KING]="king";type[Chess::QUEEN]="queen";
    type[Chess::KNIGHT]="knight";type[Chess::BISHOP]="bishop";
    type[Chess::ROOK]="rook";type[Chess::PAWN]="pawn";

    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++){
            if (Chess::type[i][j] != Chess::NOTYPE)
                pos[Chess::color[i][j]][Chess::type[i][j]].push_back(Pos(i, j));
        }
    QString ans;
    QTextStream stream(&ans);

    int seq[2]={Chess::WHITE,Chess::BLACK};
    if(turnColor==Chess::BLACK)
        qSwap(seq[0],seq[1]);
    for(int i=0;i<2;i++){
        int k=seq[i];
        stream<<color[k]<<'\n';
        for(int j=0;j<6;j++){
            if(pos[k][j].size()){
            stream<<type[j]<<' '<<pos[k][j].size();
            for(Pos p:pos[k][j]){
               stream<<' '<<(char)(p.first+'a')
                    <<p.second+1;
            }
            stream<<'\n';
            }
        }
    }
    stream.flush();
    return ans;
}

void ChessWidget::setColor(bool white)
{
    if (white)
        color = Chess::WHITE;
    else
        color = Chess::BLACK;
}

void ChessWidget::setTurnColor(bool white)
{
    if (white)
        turnColor = Chess::WHITE;
    else
        turnColor = Chess::BLACK;
}

void ChessWidget::startGame()
{
    end = false;
    repaint();
    emit signalTurn(int(turnColor == Chess::WHITE));
}

void ChessWidget::movePiece(int x1, int y1, int x2, int y2, bool self)
{//复杂
    int ret = Chess::moveTo(x1, y1, x2, y2, turnColor);
    if (ret) {
        if (self) {//非行棋端调用时self=0,避免循环
            tm->stop();
            selX = -1;
            selY = -1;

            //判断是否有兵需要升变
            int x,y;bool mark=false;
            Chess::Type t=Chess::NOTYPE;
            for(int i=0;i<8;i++){
                if(Chess::color[i][0]==Chess::BLACK && Chess::type[i][0]==Chess::PAWN){
                    if(promotionDialog->exec()==QDialog::Accepted){
                        t=promotionDialog->getType();
                        promotion(i,0,t);
                        mark=true;
                        x=i;y=0;
                        break;//一次只可能升变一个兵
                    }
                }
                if(Chess::color[i][7]==Chess::WHITE && Chess::type[i][7]==Chess::PAWN){
                    if(promotionDialog->exec()==QDialog::Accepted){
                        t=promotionDialog->getType();
                        promotion(i,7,t);
                        mark=true;
                        x=i;y=7;
                        break;//一次只可能升变一个兵
                    }
                }
            }
             signalMove(x1, y1, x2, y2);//向非行棋端发送信号更新移动
             if(mark)
                 emit signalPromote(x,y,t);
             //行棋端在此判断是否胜出或和棋
             if(Chess::checkWin(color)) {
                 setState(State::WIN);
                 setEnd(true);    //向非行棋端发送信号更新状态
                 return;
             }
             else if(Chess::checkDraw(Chess::Color(1 - turnColor))){
                 setState(State::DRAW);
                 setEnd(true);
                 return;
             }
        }
        turn();
        repaint();
    }
}

bool ChessWidget::isWhite()
{
    return color==Chess::WHITE;
}

bool ChessWidget::isWhiteTurn()
{
    return turnColor==Chess::WHITE;
}

State ChessWidget::getState()
{
    return state;
}

void ChessWidget::setState(State s)
{
    state=s;
}

void ChessWidget::promotion(int x, int y, Chess::Type t)
{
    if(Chess::type[x][y]!=Chess::PAWN) return;
    Chess::type[x][y]=t;
    repaint();
    emit signalTurn(int(turnColor == Chess::WHITE));
}
void ChessWidget::setEnd(bool net)
{
    end=true;
    emit signalEnd(net);
}

void ChessWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.save();
    p.drawImage(0, 0, QImage(":/res/res/bg.png"));
    p.restore();

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (Chess::type[i][j] != Chess::NOTYPE) {
                int x = i, y = j;
                if (x == selX && y == selY && (!end && !selShow))
                    continue;
                y = 7 - j;


                QString file = QString(":/res/res/") +
                               (Chess::color[i][j] == Chess::WHITE ? "w" : "b") +
                               QString::number(Chess::type[i][j]) + ".png";
                QPixmap image(file);
                p.drawPixmap(rects[x][y],image);
            }
        }
    }
}

void ChessWidget::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton){
        //qDebug()<<end<<turnColor<<color;
        if(end || turnColor!=color) return;

        if (selX != -1) {//如果已经有棋子被选中
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    int x = i, y = j;
                    y = 7 - j;

                    if (rects[x][y].contains(ev->pos())) {
                        if (Chess::color[i][j] == turnColor) {
                            selX = i;
                            selY = j;
                        }
                        else movePiece(selX, selY, i, j);
                        return;
                    }
                }
            }
        }
        else {
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (Chess::color[i][j] == turnColor) {
                        int x = i, y = j;
                        y = 7 - j;
                        if (rects[x][y].contains(ev->pos())) {
                            selX = i;
                            selY = j;
                            selShow = false;
                            tm->start();
                            repaint();
                            return;
                        }
                    }
                }
            }
        }
        selX = -1;
        selY = -1;
    }
    else QWidget::mousePressEvent(ev);
}

void ChessWidget::turn()
{
    turnColor = Chess::Color(1 - turnColor);
    emit signalTurn(int(turnColor == Chess::WHITE));
}
