#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QFileDialog>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionopen->setEnabled(false);
    ui->actionBack->setEnabled(false);
    ui->actionNext->setEnabled(false);
    ui->actionPlay->setEnabled(false);
    ui->actionReset->setEnabled(false);
    ui->actionStop->setEnabled(false);
    //初始化计时器
    tm=new QTimer(this);    
    tm->stop();
    tm->setInterval(1000);
    //场景设置
    scene = new QGraphicsScene(-300,-200,600,200);
    ui->View->setScene(scene);
    ui->View->setCursor(Qt::CrossCursor); //设置鼠标
    ui->View->setMouseTracking(true);
    ui->View->setDragMode(QGraphicsView::RubberBandDrag);

    this->setCentralWidget(ui->View);
    //工具栏
    lcdTime = new QLCDNumber(this);
    lcdTime->setMinimumSize(30,30);
    lcdTime->setDigitCount(3);
    lcdTime->display(0);
    ui->mainToolBar->addWidget(lcdTime);
    //状态栏
    labItemInfo=new QLabel("");
    labItemInfo->setMinimumWidth(200);
    ui->statusBar->addWidget(labItemInfo);
    //信号-槽连接
    connect(ui->View,SIGNAL(mouseClicked(QPoint)),
                     this, SLOT(on_mouseClicked(QPoint)));
    connect(ui->View,SIGNAL(mouseDoubleClick(QPoint)),
                     this, SLOT(on_mouseDoubleClick(QPoint)));
    connect(ui->View,SIGNAL(keyPress(QKeyEvent*)),
                     this, SLOT(on_keyPress(QKeyEvent*)));
    connect(tm,SIGNAL(timeout()),
            this,SLOT(on_timer_timeout()));
    connect(this,&MainWindow::time_changed,
            this,&MainWindow::on_timeChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionIni_triggered()
{
    InputDialog* input = new InputDialog();
    if(input->exec()==QDialog::Accepted){
        row=input->rowNum(); col=input->colNum();
        outPos=input->outputPos(); inPosSet=input->inputPos();
        Wash=input->wash();
        //错误检查1
        bool valid=true;
        if(row<=3&&col<=3)
            valid=false;
        if(1<outPos.first && outPos.first<row &&\
                1<outPos.second && outPos.second<col)
            valid=false;
        for(const QPair<int,int>&pos:inPosSet){
            if(1<pos.first && pos.first<row &&\
                    1<pos.second && pos.second<col){
                valid=false;break;
            }
            if(pos==outPos){
                valid=false;break;
            }
        }
        if(Wash)
            if(outPos==QPair<int,int>(1,col)||outPos==QPair<int,int>(row,1)||
                    inPosSet.contains(QPair<int,int>(row,1))|| inPosSet.contains(QPair<int,int>(1,col)) )
                valid=false;

        if(valid){
            opMap.clear();liquid.clear();
            opMusic.clear();rectDirt.clear();
            rectPos.clear();rect.clear();
            toWash.clear();Washing.clear();
            state=State::Begin;
            Error=false;
            seqNum=t=Maxt=0;//Maxt在preProcess中确定,在此初始化
            emit time_changed(t);

            ui->actionopen->setEnabled(true);
            ui->actionBack->setEnabled(false);
            ui->actionNext->setEnabled(false);
            ui->actionPlay->setEnabled(false);
            ui->actionReset->setEnabled(false);
            ui->actionStop->setEnabled(false);
            ui->View->resetTransform();
            viewInit();
        }
        else{
            QString dlgTitle("Error");
            QString strInfo("输入数据不合法！注意:\n行数和列数不能同时小于等于3;\n输入、输出端口应当在芯片的边界上且不能重合.");
            QMessageBox::warning(this,dlgTitle,strInfo);
        }
    }
}

void MainWindow::openText(QString Filename)
{

    QFile f(Filename);
    if(!f.exists()||!f.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    QTextStream stream(&f);
    bool warn=false;

    while(!f.atEnd()){
        QString str=f.readLine();
        str.replace(" ",",");
        str.replace(";","");
        str.replace("\n","");

        QStringList strList = str.split(",");

        int time=strList[1].toInt();

        if(strList[0]!="Mix"){
            QVector<int> value;
            if(strList[0]=="Input"){
                value.push_back(Op::Input);
                if(!inPosSet.contains(QPair<int,int>(strList[2].toInt(),strList[3].toInt()))){
                    warn=true;break;//错误检查2
                }
            }
            else if (strList[0]=="Output"){
                value.push_back(Op::Output);
                if(QPair<int,int>(strList[2].toInt(),strList[3].toInt())!=outPos){
                    warn=true;break;//错误检查2
                }
            }
            else if (strList[0]=="Split")
                value.push_back(Op::Split);
            else if (strList[0]=="Merge")
                value.push_back(Op::Merge);
            else if(strList[0]=="Move")
                value.push_back(Op::Move);

            for(int i=2;i<strList.size();i++)
                value.push_back(strList[i].toInt());
            opMap.insert(time,value);
        }
        else if(strList[0]=="Mix"){//特判，将mix转换成系列move
            for(int i=2;i+3<strList.size();i+=2){
                QVector<int>value;
                value.push_back(Op::Move);

                for(int j=i;j<=i+3;j++){
                    value.push_back(strList[j].toInt());
                }
                opMap.insert(time,value);
                time++;
            }
        }
        else qDebug()<<"输入文件解析失败";
    }
    f.close();
    if(warn){
        QString dlgTitle("Error");
        QString strInfo("输入数据不合法！注意:\n 输入、输出端口应当在芯片的边界上.");
        QMessageBox::warning(this,dlgTitle,strInfo);
    }
    else {
        preProcess();
        ui->actionNext->setEnabled(true);
        if(!Wash){
            ui->actionPlay->setEnabled(true);

        }
        ui->actionReset->setEnabled(true);
        ui->actionopen->setEnabled(false);
    }
}


void MainWindow::preProcess()
{
    int r=25;
    QMap<int,QVector<int>>::iterator iter;
    for(iter=opMap.begin();iter!=opMap.end();iter++){
        const QVector<int>&value=iter.value();
        int t=iter.key(),op=value[0];

        Maxt=qMax(Maxt,t);

        if(op== Op::Input){
            int x=value[1],y=value[2];
            Liquid* tmp=new Liquid;
            Corrd corrd(x,y);
            tmp->vis.insert(corrd);
            tmp->item=new QGraphicsEllipseItem(-r,-r,2*r,2*r);
            tmp->item->setPos(rectPos[x][y]);
            tmp->item->setData(NoId,++seqNum);

            liquid[1+t].insert(corrd,tmp);//t时刻input, t+1时刻出现
        }
        else if(op==Op::Move){
            Corrd corrd1(value[1],value[2]),corrd2(value[3],value[4]);

            Liquid* tmp_move=new Liquid;

            Liquid* pre=findObject(t,corrd1);
            tmp_move->vis=pre->vis;
            tmp_move->vis.insert(corrd2);

            tmp_move->item=new QGraphicsEllipseItem;
            tmp_move->item->setRect(pre->item->rect());
            tmp_move->item->setPos(rectPos[value[3]][value[4]]);
            tmp_move->item->setData(NoId,pre->item->data(NoId));

            liquid[1+t].insert(corrd2,tmp_move);
            opMusic.insert(1+t,MusicOp::Move);
        }
        else if(op==Op::Merge){
            Corrd corrd1(value[1],value[2]),corrd2(value[3],value[4]);//起始坐标
            findObject(t,corrd1);findObject(t,corrd2);
            int x=(value[1]+value[3])/2,y=(value[2]+value[4])/2;
            Corrd corrd(x,y);//终末坐标
            //第一阶段
            Liquid* tmp_merging=new Liquid;
            tmp_merging->vis.insert(corrd1);
            tmp_merging->vis.insert(corrd2);
            tmp_merging->vis.insert(corrd);

            if(value[1]==value[3])//在同一行
                tmp_merging->item=new QGraphicsEllipseItem(-3*r,-r,6*r,2*r);

            else //value[2]==value[4]//在同一列
                tmp_merging->item=new QGraphicsEllipseItem(-r,-3*r,2*r,6*r);

            tmp_merging->item->setPos(rectPos[x][y]);
            tmp_merging->item->setData(NoId,++seqNum);

            liquid[1+t].insert(corrd,tmp_merging);

            //第二阶段
            Liquid* tmp_merged=new Liquid;
            tmp_merged->vis=tmp_merging->vis;

            tmp_merged->item=new QGraphicsEllipseItem(-1.5*r,-1.5*r,3*r,3*r);
            tmp_merged->item->setPos(rectPos[x][y]);
            tmp_merged->item->setData(NoId,tmp_merging->item->data(NoId));

            liquid[2+t].insert(corrd,tmp_merged);

            opMusic.insert(t+2,MusicOp::Merge);
            //qDebug()<<"merged"<<x<<y;
        }
        else if(op==Op::Split){
            Corrd corrd(value[1],value[2]);
            Corrd corrds[2]{Corrd(value[3],value[4]),Corrd(value[5],value[6])};
            //第一阶段
            Liquid* tmp_spliting=new Liquid,*pre=findObject(t,corrd);
            tmp_spliting->vis=pre->vis;

            if(value[3]==value[5])//在同一行
                tmp_spliting->item=new QGraphicsEllipseItem(-3*r,-r,6*r,2*r);

            else //value[4]==value[6]//在同一列
                tmp_spliting->item=new QGraphicsEllipseItem(-r,-3*r,2*r,6*r);
            tmp_spliting->item->setPos(rectPos[value[1]][value[2]]);
            tmp_spliting->item->setData(NoId,pre->item->data(NoId));

            for(int i=0;i<2;i++)
                tmp_spliting->vis.insert(corrds[i]);
            qDebug()<<corrd<<tmp_spliting;
            liquid[t+1].insert(corrd,tmp_spliting);
            //第二阶段
            Liquid** tmp_splited=new Liquid*[2];
            for(int i=0;i<2;i++){
                tmp_splited[i]=new Liquid;
                tmp_splited[i]->vis.insert(corrds[i]);
                tmp_splited[i]->item=new QGraphicsEllipseItem(-r,-r,2*r,2*r);
                tmp_splited[i]->item->setPos(rectPos[value[2*i+3]][value[2*i+4]]);
                tmp_splited[i]->item->setData(NoId,++seqNum);

                liquid[2+t].insert(corrds[i],tmp_splited[i]);
            }

            opMusic.insert(1+t,MusicOp::Spliting);
            opMusic.insert(2+t,MusicOp::Splited);
        }
        else if(op==Op::Output)
            continue;
    }
    Maxt++;
    for(int i=1;i<Maxt;i++){
        Error=!stateValid(i);
        if(Error){
            Maxt=i;
            break;
        }
    }
    for(int i=1;i<=Maxt;i++){
        if(rectDirt.contains(i-1))//复制前一秒的污染状态
            rectDirt[i]=rectDirt[i-1];
        for(Liquid* iter:liquid.value(i).values()){

            for(Corrd pos:iter->vis)
                rectDirt[i][pos].insert(iter->item->data(NoId).toInt());

            iter->item->setBrush(Qt::yellow);
            iter->item->setZValue(1);

            iter->item->setData(InfoId,"液滴");
            iter->item->setFlags(QGraphicsItem::ItemIsSelectable
                                       | QGraphicsItem::ItemIsFocusable);
        }
    }

    if(Wash){
        QMap<int,QMap<Corrd,QSet<int>>> tmpDirt=rectDirt;
        for(int i=1;i<=Maxt;i++){
            QMap<Corrd,QSet<int>>::iterator j;
            for(j=tmpDirt[i].begin();j!=tmpDirt[i].end();j++){
                if(j.value().size()==2){//发现将被二次污染的电极
                    Corrd tmpPos=j.key();
                    int k,seq=0;
                    for(k=i-1;k>=1;k--){//找到电极最初被污染的时刻
                        if(!tmpDirt[k-1].count(tmpPos)&& !tmpDirt[k][tmpPos].values().isEmpty()){
                            seq=tmpDirt[k][tmpPos].values().at(0);
                            break;
                        }

                    }//可能的清洗时间是[k+2,i-2]
                    if(seq && i-2>=k+2){
                        for(int r=k;r<=Maxt;r++)
                            tmpDirt[r][tmpPos].remove(seq);
                        toWash.insert(k+2,QPair<int,Corrd>(i-2,tmpPos));
                    }
                }
            }
        }
    }
}


void MainWindow::playHandle(int flag)
{
    if(flag!=1 && flag!=-1) return;
    if(state==State::Wash) {
        ui->actionBack->setEnabled(false);
        washHandle();
        return;
    }
    if(state==State::End) {
        if(flag==1)
            return;
        else {
            t=Maxt;
            state=State::Play;
            if(!Wash)
                ui->actionPlay->setEnabled(true);
            ui->actionNext->setEnabled(true);
        }
    }
    if(state==State::Begin){
        if(flag==1){
            t=0;
            state=State::Play;
            ui->actionBack->setEnabled(true);
        }
        else return;
    }

    t+=flag; emit time_changed(t);
    for(Liquid* l:liquid.value(t-flag))
        scene->removeItem(l->item);
    for(Liquid* l:liquid.value(t))
        scene->addItem(l->item);
    playMusic(t);

    if(t==Maxt){
        state=State::End;
        ui->actionNext->setEnabled(false);
        ui->actionPlay->setEnabled(false);
        if(Error){//错误检测3
            QString dlgTitle("Error");
            QString strInfo("输入数据不合法！\n 不满足DMFB移动约束！");
            QMessageBox::warning(this,dlgTitle,strInfo);
        }
    }
    if(t==0){
        state=State::Begin;
        ui->actionBack->setEnabled(false);
    }
    if(Wash&&flag==1){
        checkWash(t);
    }
    //qDebug()<<"t ="<<t;
}

void MainWindow::washHandle()
{

    QSet<Corrd> tmp_unWashable = UnWashable;

    for(Corrd pos:liquid.value(t).keys()){
        for(int i=0;i<9;i++){
            int x=pos.first+xAround[i],y=pos.second+yAround[i];
            if(1<=x && x<=row && 1<=y && y<=col)
                tmp_unWashable.insert(QPair<int,int>(x,y));
        }
    }
    Solver solver(washDripPos,target,tmp_unWashable,row,col);
    if(washed)
        solver=Solver(washDripPos,QPair<int,int>(1,col),tmp_unWashable,row,col);

    if(solver.BFS()){
        washDripPos = solver.route[1];
        washDrip->setPos(rectPos[washDripPos.first][washDripPos.second]);
        scene->removeItem(washDrip);
        scene->addItem(washDrip);
        QSound::play("sound/move.wav");
        QList<int> removedDirt=rectDirt[t][washDripPos].toList();
        for(int i=t;i<=Maxt;i++){
            for(int j:removedDirt)
                rectDirt[i][washDripPos].remove(j);
        }
    }


    else {
        QString dlgTitle("Error");
        QString strInfo("寻路失败!请开放部分电极!");
        QMessageBox::warning(this,dlgTitle,strInfo);
        qDebug()<<"Error 寻路失败!";
        return;
    }
    qDebug()<<washDripPos;
    if(washDripPos==target) washed=true;
    else if(washDripPos==QPair<int,int>(1,col)){
        ui->actionBack->setEnabled(true);
        scene->removeItem(washDrip);
        state=State::Play;
    }
}

void MainWindow::checkWash(int time)
{
    QSet<Corrd> tmp_unWashable = UnWashable;
    for(Corrd pos:liquid.value(t).keys()){
        qDebug()<<pos;
        for(int i=0;i<9;i++){
            int x=pos.first+xAround[i],y=pos.second+yAround[i];
            if(1<=x && x<=row && 1<=y && y<=col){
                tmp_unWashable.insert(QPair<int,int>(x,y));
            }
        }

    }
    if(toWash.count(time))
        for(QPair<int,Corrd> iter:toWash.values(time)){
            Washing.insert(iter);
        }
    QList<QPair<int,Corrd>> toRemove;
    for(QPair<int,Corrd> iter:Washing){
        Solver solve1(QPair<int,int>(row,1),iter.second,tmp_unWashable,row,col);
        Solver solve2(iter.second,QPair<int,int>(1,col),tmp_unWashable,row,col);
        if(!tmp_unWashable.contains(QPair<int,int>(row,1))&&solve1.BFS()&&solve2.BFS()) {
            washed=false;
            target=iter.second;
            washDripPos=QPair<int,int>(row,1);
            state=State::Wash;

            washDrip=new QGraphicsEllipseItem(-r,-r,2*r,2*r);
            washDrip->setPos(rectPos[row][1]);
            washDrip->setData(InfoId,"清洗液滴");
            washDrip->setBrush(QBrush(QColor(153,255,255)));
            washDrip->setZValue(1);
            scene->addItem(washDrip);

            toRemove.push_back(iter);
            continue;
        }

        if(time>=iter.first){//时间已过，说明无法清洗
            toRemove.push_back(iter);
            QString dlgTitle("Warning");
            QString strInfo(tr("坐标(%1, %2)处的电极清洗失败!").arg(iter.second.first).arg(iter.second.second));
            QMessageBox::warning(this,dlgTitle,strInfo);
            continue;
        }

    }
    for(int i=0;i<toRemove.size();i++)
        Washing.remove(toRemove[i]);
}

void MainWindow::playMusic(int time)
{
    if(opMusic.contains(time)){
        foreach(MusicOp p,opMusic.values(time)){
            switch (p) {
            case MusicOp::Move:
                QSound::play("sound/move.wav");break;
            case MusicOp::Merge:
                QSound::play("sound/merge.wav");break;
            case MusicOp::Spliting:
                QSound::play("sound/spliting.wav");break;
            case MusicOp::Splited:
                QSound::play("sound/splited.wav");break;
            default: break;
            }
        }
    }
}

bool MainWindow::stateValid(int time)
{
    QList<Corrd> list = liquid.value(time).keys();
    if(list.size()<2) return true;
    for(QList<Corrd>::iterator i=list.begin();i+1!=list.end();i++)
        for(QList<Corrd>::iterator j=i+1;j!=list.end();j++){
            if(pow(i->first-j->first,2)+pow(i->second-j->second,2)<3){
                return false;
            }
        }
    return true;
}

Liquid* MainWindow::findObject(int rank,Corrd corrd)
{         //更新没有动过的液滴
    int tmp=rank;
    while(!liquid.value(tmp).contains(corrd)){
        tmp--;
        if(tmp<0) return nullptr;
    }
    Liquid* find=liquid.value(tmp).value(corrd);

    for (int i=tmp+1;i<=rank;i++)
        liquid[i].insert(corrd,find);
    return find;
}

void MainWindow::viewInit()
{
    scene->clear();
    //绘制中间层
    rect.resize(row+1);
    rectPos.resize(row+1);
    for(int i=1;i<=row;i++){
        rect[i].resize(1+col); rectPos[i].resize(1+col);
        for(int j=1;j<=col;j++){
            rect[i][j]=new QGraphicsRectItem(-width/2,-height/2,width,height);
            rect[i][j]->setFlags(QGraphicsItem::ItemIsSelectable
                                 | QGraphicsItem::ItemIsFocusable);

            QPointF point=ui->View->mapToScene(extent+width*j,extent+height*i);
            rect[i][j]->setPos(point);
            scene->addItem(rect[i][j]);
            rectPos[i][j]=point;
            //qDebug()<<rectPos[i][j].x()<<rectPos[i][j].y();

            rect[i][j]->setData(rowId,i);
            rect[i][j]->setData(colId,j);
            rect[i][j]->setData(InfoId,"电极");
        }
    }
    //绘制出口
    QGraphicsRectItem* out=new QGraphicsRectItem(-width/2,-height/2,width,height);
    out->setFlags(QGraphicsItem::ItemIsSelectable
                  | QGraphicsItem::ItemIsFocusable);
    out->setBrush(QBrush(Qt::blue));
    int x=outPos.second,y=outPos.first;
    if(x==1) --x;
    else if(x==col) ++x;
    else if(y==1) --y;
    else ++y;
    out->setPos(ui->View->mapToScene(QPoint(extent+width*x,extent+height*y)));
    scene->addItem(out);
    out->setData(InfoId,"出口");
    //绘制入口
    QVector<QGraphicsRectItem*>inVec(inPosSet.size());
    int i=0;
    for(const QPair<int,int>&pos:inPosSet){
        inVec[i]=new QGraphicsRectItem(-width/2,-height/2,width,height);
        inVec[i]->setFlags(QGraphicsItem::ItemIsSelectable
                           | QGraphicsItem::ItemIsFocusable);
        inVec[i]->setBrush(QBrush(Qt::yellow));
        x=pos.second;y=pos.first;
        if(x==1) --x;
        else if(x==col) ++x;
        else if(y==1) --y;
        else ++y;
        inVec[i]->setPos(ui->View->mapToScene(QPoint(extent+width*x,extent+height*y)));
        scene->addItem(inVec[i]);
        inVec[i]->setData(InfoId,"入口");

        i++;
    }//清洗液滴
    if(Wash){
      QGraphicsRectItem* Wash[2];
      for(int i=0;i<2;i++){
          Wash[i]=new QGraphicsRectItem(-width/2,-height/2,width,height);
          Wash[i]->setFlags(QGraphicsItem::ItemIsSelectable
                            | QGraphicsItem::ItemIsFocusable);
          scene->addItem(Wash[i]);
    }
     Wash[0]->setBrush(QBrush(QColor(255,69,180)));
     Wash[0]->setData(InfoId,"清洗出口");
     Wash[0]->setPos(ui->View->mapToScene(QPoint(extent+width*(col+1),extent+height)));

     Wash[1]->setBrush(QBrush(QColor(0,191,255)));
     Wash[1]->setData(InfoId,"清洗入口");
     Wash[1]->setPos(ui->View->mapToScene(QPoint(extent,extent+height*row)));
    }
}


void MainWindow::on_actionopen_triggered()
{
    QString curPath=QDir::currentPath();
    QString digTitle="Open Document";
    QString filter="txt(*.txt)";
    QString aFileName=QFileDialog::getOpenFileName(this,digTitle,curPath,filter);
    if(!aFileName.isEmpty())
        openText(aFileName);
}

void MainWindow::on_mouseClicked(QPoint point)
{
    QPointF pointScene=ui->View->mapToScene(point); //转换到Scene坐标
    QGraphicsItem  *item=nullptr;
    item=scene->itemAt(pointScene,ui->View->transform()); //获取光标下的绘图项
    if(item){
        QStringList info;
        info<<item->data(InfoId).toString();

        if (item->type()==QGraphicsRectItem::Type){
            if(!item->data(rowId).isNull()){//是电极
                info<<" 坐标("<<item->data(rowId).toString()<<" ,"
                   <<item->data(colId).toString()<<") ";
                Corrd pos(item->data(rowId).toInt(),item->data(colId).toInt());
                if(rectDirt.value(t).value(pos).size()>0){
                    const QSet<int>& tmp=rectDirt[t][pos];
                    info<<"污染数: "<<QString::number(tmp.size())<<" 污染液滴编号:";
                    for(int i:tmp)
                        info<<" "<<QString::number(i);

                }
                else {
                    info<<"污染数: 0";
                }
            }
        }
        else if(item->type()==QGraphicsEllipseItem::Type){
            if(!item->data(NoId).isNull())
                info<<" 编号: "<<item->data(NoId).toString();
        }
        labItemInfo->setText(info.join(""));
    }
}

void MainWindow::on_mouseDoubleClick(QPoint point)
{
    QPointF pointScene=ui->View->mapToScene(point); //转换到Scene坐标
    QGraphicsItem *item=nullptr;
    item=scene->itemAt(pointScene,ui->View->transform()); //获取光标下的绘图项

    if (!item) //没有绘图项
        return;

    switch (item->type())  //绘图项的类型
    {
    case    QGraphicsTextItem::Type: //文字，设置字体
    {
        QGraphicsTextItem *theItem=qgraphicsitem_cast<QGraphicsTextItem*>(item);
        QFont font=theItem->font();
        bool ok=false;
        font=QFontDialog::getFont(&ok,font,this,"设置字体");
        if (ok)
            theItem->setFont(font);
        break;
    }
    case QGraphicsRectItem::Type:{
        if(Wash && !item->data(rowId).isNull()){
            //设置清洗障碍
            QGraphicsRectItem *theItem=qgraphicsitem_cast<QGraphicsRectItem*>(item);
            QPair<int,int> pos(theItem->data(rowId).toInt(),theItem->data(colId).toInt());
            if(!UnWashable.contains(pos)){
                UnWashable.insert(pos);
                theItem->setBrush(QBrush(QColor(169,169,169)));
            }
            else {
                UnWashable.remove(pos);
                theItem->setBrush(QBrush(Qt::white));
            }
        }
    }
    }
}

void MainWindow::on_keyPress(QKeyEvent *event)
{//删除文字
    if (scene->selectedItems().count()!=1)
        return; //没有选中的绘图项，或选中的多于1个
    QGraphicsItem  *item=scene->selectedItems().at(0);
    if(item->type()==QGraphicsTextItem::Type){
        if (event->key()==Qt::Key_Delete || event->key()==Qt::Key_Backspace)//删除
            scene->removeItem(item);
    }
}

void MainWindow::on_actionExpand_triggered()
{
    ui->View->scale(1.1,1.1);
}

void MainWindow::on_actionShrink_triggered()
{
    ui->View->scale(0.9,0.9);
}

void MainWindow::on_actionNext_triggered()
{
    playHandle(1);
}

void MainWindow::on_actionBack_triggered()
{
    playHandle(-1);
}

void MainWindow::on_actionReset_triggered()
{
    while(t){
        playHandle(-1);
    }
}

void MainWindow::on_actionPlay_triggered()
{
    tm->start();
    ui->actionPlay->setEnabled(false);
    ui->actionStop->setEnabled(true);
}

void MainWindow::on_timer_timeout()
{
    playHandle(1);
    if(t==Maxt){
        tm->stop();
        ui->actionStop->setEnabled(false);
    }
}

void MainWindow::on_actionStop_triggered()
{
    tm->stop();
    ui->actionPlay->setEnabled(true);
    ui->actionStop->setEnabled(false);
}

void MainWindow::on_timeChanged(int time)
{
    lcdTime->display(time);
}

void MainWindow::on_actionRotateRight_triggered()
{
    ui->View->rotate(+30);
}

void MainWindow::on_actionRotateLeft_triggered()
{
    ui->View->rotate(-30);
}

void MainWindow::on_actionText_triggered()
{
    QString str=QInputDialog::getText(this,"输入文字","请输入文字");
    if (str.isEmpty())
        return;

    QGraphicsTextItem   *item=new QGraphicsTextItem(str);

    QFont   font=this->font();
    font.setPointSize(20);
    font.setBold(true);
    item->setFont(font);

    item->setFlags(QGraphicsItem::ItemIsMovable
                   | QGraphicsItem::ItemIsSelectable
                   | QGraphicsItem::ItemIsFocusable);
    item->setPos(0,0);
    item->setZValue(2);
    item->setData(InfoId,"文字");

    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}
