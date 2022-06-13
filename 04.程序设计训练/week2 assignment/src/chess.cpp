#include "chess.h"
#include <algorithm>
#include <QDebug>
namespace Chess {

Type type[8][8];
Color color[8][8];

void clear() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) {
            type[i][j] = NOTYPE;
            color[i][j] = NOCOLOR;
        }
}

void init() {
    clear();
    setPiece(0,0,ROOK,WHITE);setPiece(7,0,ROOK,WHITE);
    setPiece(1,0,KNIGHT,WHITE);setPiece(6,0,KNIGHT,WHITE);
    setPiece(2,0,BISHOP,WHITE);setPiece(5,0,BISHOP,WHITE);
    setPiece(3,0,QUEEN,WHITE);setPiece(4,0,KING,WHITE);
    for(int i=0;i<8;i++){
        setPiece(i,1,PAWN,WHITE);
        setPiece(i,6,PAWN,BLACK);
    }
    setPiece(0,7,ROOK,BLACK);setPiece(7,7,ROOK,BLACK);
    setPiece(1,7,KNIGHT,BLACK);setPiece(6,7,KNIGHT,BLACK);
    setPiece(2,7,BISHOP,BLACK);setPiece(5,7,BISHOP,BLACK);
    setPiece(3,7,QUEEN,BLACK);setPiece(4,7,KING,BLACK);
}

inline void setPiece(int i, int j, Type t, Color c){
    type[i][j] = t;
    color[i][j] = c;
}

bool checkWin(Color c)
{//检查c王是否活着
    bool kingAlive=false;
    for (int i = 0; i < 8 && !kingAlive; ++i)
        for (int j = 0; j < 8 && !kingAlive; ++j)
            if (type[i][j] == KING && color[i][j] == Color(1 - c))
                kingAlive = true;
    if(!kingAlive) return true;
    else return false;
}

bool checkDraw(Color c)
{
    qDebug()<<"check";
    int KING_X,KING_Y;
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(color[i][j]==c&&type[i][j]==KING){
                qDebug()<<i<<j;
                if(checkAttack(i,j,Color(1-c)))//如果王正在被将
                {qDebug()<<"king attacked!";
                    return false;}
                for(Pos p:possibleMove(i,j)){
                    type[p.first][p.second] = type[i][j];
                    color[p.first][p.second] = color[i][j];
                    type[i][j] = NOTYPE;
                    color[i][j] = NOCOLOR;
                    bool mark=false;
                    if(!checkAttack(p.first,p.second,Color(1-c)))//有移动后王不受攻击的方法
                        mark=true;
                    type[i][j] = type[p.first][p.second];
                    color[i][j] = color[p.first][p.second];
                    type[p.first][p.second] = NOTYPE;
                    color[p.first][p.second] = NOCOLOR;
                    if(mark){
                        qDebug()<<p<<"king escaped!";
                        return false;
                    }
                }
                KING_X=i;
                KING_Y=j;
                }
    qDebug()<<KING_X<<KING_Y;
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(color[i][j]==c&&type[i][j]!=KING)
                for(Pos p:possibleMove(i,j)){
                    type[p.first][p.second] = type[i][j];
                    color[p.first][p.second] = color[i][j];
                    type[i][j] = NOTYPE;
                    color[i][j] = NOCOLOR;
                    bool mark=false;
                    if(!checkAttack(KING_X,KING_Y,Color(1-c)))//有移动后王不受攻击的方法
                        mark=true;
                    type[i][j] = type[p.first][p.second];
                    color[i][j] = color[p.first][p.second];
                    type[p.first][p.second] = NOTYPE;
                    color[p.first][p.second] = NOCOLOR;
                    if(mark){
                        qDebug()<<"king excaped"<<p<<i<<j;
                        return false;
                    }
              }
    return true;
}


std::vector<Pos> possibleMove(int x, int y)
{
    vector<Pos> ans;
    static const int dx1[4] = {0, -1, 1, 0}, dy1[4] = {1, 0, 0, -1}; // rook
    static const int dx2[4] = {1, 1, -1, -1}, dy2[4] = {1, -1, 1, -1}; // bishop,
    static const int dx3[8] = {2, 1, -1, -2, -2, -1, 1, 2}, dy3[8] = {1, 2, 2, 1, -1, -2, -2, -1}; // knight
    static const int dx4[8]={0,-1,1,0,1,1,-1,-1},dy4[8]={1,0,0,-1,1,-1,1,-1}; //king,queen
    if(type[x][y]==KING){
        for(int i=0;i<8;i++){
            int fx=x+dx4[i],fy=y+dy4[i];
            //qDebug()<<fx<<fy;
            if(fx>=0 && fy>=0 && fx<=7 && fy<=7 && color[fx][fy]!=color[x][y])
                ans.push_back(Pos(fx,fy));
      }
    }
    else if(type[x][y]==KNIGHT){
        for (int k = 0; k < 8; ++k) {
            int fx = x + dx3[k],  fy = y + dy3[k];
            if(fx>=0 && fy>=0 && fx<=7 && fy<=7 && color[fx][fy]!=color[x][y])
                ans.push_back(Pos(fx, fy));
        }
    }
    else if(type[x][y]==BISHOP){
        for(int i=0;i<4;i++){
            int fx=x+dx2[i], fy=y+dy2[i];
            while (0<=fx && fx<=7 && 0<=fy && fy<=7 && color[fx][fy]==NOCOLOR){
                ans.push_back(Pos(fx,fy));
                fx += dx2[i]; fy+=dy2[i];
            }
            if(fx>=0 && fy>=0 && fx<=7 && fy<=7 && color[fx][fy]!=color[x][y])
                ans.push_back(Pos(fx,fy));
        }
    }
    else if(type[x][y]==ROOK){
        for(int i=0;i<4;i++){
            int fx=x+dx1[i], fy=y+dy1[i];
            while (0<=fx && fx<=7 && 0<=fy && fy<=7 && color[fx][fy]==NOCOLOR){
                ans.push_back(Pos(fx,fy));
                fx += dx1[i]; fy+=dy1[i];
            }
            if(fx>=0 && fy>=0 && fx<=7 && fy<=7 && color[fx][fy]!=color[x][y])
                ans.push_back(Pos(fx,fy));
        }
    }
    else if(type[x][y]==QUEEN){
        for(int i=0;i<8;i++){
            int fx=x+dx4[i], fy=y+dy4[i];
            while (0<=fx && fx<=7 && 0<=fy && fy<=7 && color[fx][fy]==NOCOLOR){
                ans.push_back(Pos(fx,fy));
                fx += dx4[i]; fy+=dy4[i];
            }
            if(fx>=0 && fy>=0 && fx<=7 && fy<=7 && color[fx][fy]!=color[x][y])
                ans.push_back(Pos(fx,fy));
        }
    }
    else if(type[x][y]==PAWN){
        int fy=y;
        if(color[x][y]==WHITE) fy+=1;
        else if(color[x][y]==BLACK) fy-=1;
        if(fy<0 || fy>7) return ans;


        if(x<=6 && color[x+1][fy]==1-color[x][y]) ans.push_back(Pos(x+1,fy));
        if(x>=1 && color[x-1][fy]==1-color[x][y]) ans.push_back(Pos(x-1,fy));
        if(color[x][fy]==NOCOLOR)
        {
            ans.push_back(Pos(x,fy));
            //第一步可以走两格
            if(color[x][y]==WHITE && y==1) fy+=1;
            else if(color[x][y]==BLACK && y==6) fy-=1;
            if(color[x][fy]==NOCOLOR) ans.push_back(Pos(x,fy));
        }
    }
    //qDebug()<<ans;
    return ans;
}
int moveTo(int x1, int y1, int x2, int y2, Color c)
{
    if(color[x1][y1]!=c)
        return 0;
    if(checkCastling(x1,y1,x2,y2,c)){
        //qDebug()<<"王车易位成功!";
        return 1;
    }
    for(Pos p:possibleMove(x1,y1)){
        if(p.first==x2 && p.second==y2){
            type[x2][y2] = type[x1][y1];
            color[x2][y2] = color[x1][y1];
            type[x1][y1] = NOTYPE;
            color[x1][y1] = NOCOLOR;
            return 1;
        }
    }
    return 0;
}

bool checkAttack(int x, int y, Color c)
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (color[i][j] == c)
                for (auto &p : possibleMove(i, j))
                    if (p.first==x && p.second==y)
                        return true;
    return false;
}

bool checkCastling(int x1, int y1, int x2, int y2, Color c)
{
    if(x1!=4||(y1!=0&&y1!=7)||type[x1][y1]!=KING||y2!=y1)
        return false;
    if(c==WHITE){
        if(x2==2&&type[0][0]==ROOK&&color[0][0]==c){
            for(int i=1;i<=3;i++)//检查是否有障碍
                if(type[i][0]!=NOTYPE)
                    return false;
            for(int i=2;i<=4;i++)//检查路上是否被攻击
                if(checkAttack(i,0,Color(1-c)))
                    return false;
            setPiece(0,0,NOTYPE,NOCOLOR);
            setPiece(4,0,NOTYPE,NOCOLOR);
            setPiece(2,0,KING,c);
            setPiece(3,0,ROOK,c);
            return true;
        }
        else if(x2==6&&type[7][0]==ROOK&&color[7][0]==c){
            for(int i=5;i<=6;i++)//检查是否有障碍
                if(type[i][0]!=NOTYPE)
                    return false;
            for(int i=4;i<=6;i++)//检查路上是否被攻击
                if(checkAttack(i,0,Color(1-c)))
                    return false;
            setPiece(7,0,NOTYPE,NOCOLOR);
            setPiece(4,0,NOTYPE,NOCOLOR);
            setPiece(6,0,KING,c);
            setPiece(5,0,ROOK,c);
            return true;
        }
        else return false;
    }
    else if(c==BLACK){
        if(x2==2&&type[0][7]==ROOK&&color[0][7]==c){
            for(int i=1;i<=3;i++)//检查是否有障碍
                if(type[i][7]!=NOTYPE)
                    return false;
            for(int i=2;i<=4;i++)//检查路上是否被攻击
                if(checkAttack(i,7,Color(1-c)))
                    return false;
            setPiece(0,7,NOTYPE,NOCOLOR);
            setPiece(4,7,NOTYPE,NOCOLOR);
            setPiece(2,7,KING,c);
            setPiece(3,7,ROOK,c);
            return true;
        }
        else if(x2==6&&type[7][7]==ROOK&&color[7][7]==c){
            for(int i=5;i<=6;i++)//检查是否有障碍
                if(type[i][7]!=NOTYPE)
                    return false;
            for(int i=4;i<=6;i++)//检查路上是否被攻击
                if(checkAttack(i,7,Color(1-c)))
                    return false;
            setPiece(7,7,NOTYPE,NOCOLOR);
            setPiece(4,7,NOTYPE,NOCOLOR);
            setPiece(6,7,KING,c);
            setPiece(5,7,ROOK,c);
            return true;
        }
        else return false;
    }
    return false;
}

}
