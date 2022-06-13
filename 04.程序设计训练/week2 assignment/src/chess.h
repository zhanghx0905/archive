#ifndef CHESS_H
#define CHESS_H
#include <vector>
#include <QPair>

using std::vector;
typedef QPair<int,int> Pos;
//棋盘空间
namespace Chess {
enum Type{
    KING,QUEEN,ROOK,KNIGHT,BISHOP,PAWN,NOTYPE
};

enum Color {
    WHITE, BLACK,NOCOLOR
};
extern Type type[8][8];
extern Color color[8][8];
extern void clear();
extern void init();
extern bool checkWin(Color c);//检查c方是否获胜
extern bool checkDraw(Color c);//检查c方是否有子可走
extern bool checkAttack(int x,int y,Color c);//c方在(x,y)位置是否能够攻击
extern bool checkCastling(int x1, int y1, int x2, int y2, Color c);//检查c方是否触发王车易位
extern vector<Pos> possibleMove(int x, int y);
//若(x1, y1)棋子移动到(x2, y2)合法，返回1；若不合法，返回0
extern int moveTo(int x1, int y1, int x2, int y2, Color c);
 //将(i, j)位置上赋{t,c}对应的棋子 \p
extern inline void setPiece(int i, int j, Type t, Color c);

}

#endif // CHESS_H
