#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent){
    setFixedSize(BoardMargin*2+BlockSize*boardsize,BoardMargin*2+BlockSize*boardsize);
    setMouseTracking(true);
    QMenu *gameMenu = menuBar()->addMenu(tr("选择模式"));
    QAction *actionPVP = new QAction("人人对战", this);
    connect(actionPVP, SIGNAL(triggered()), this, SLOT(PVPInit()));
    gameMenu->addAction(actionPVP);
    QAction *actionPVE = new QAction("人机对战", this);
    connect(actionPVE, SIGNAL(triggered()), this, SLOT(PVEInit()));
    gameMenu->addAction(actionPVE);
    QAction *actionEVE = new QAction("AI VS AI", this);
    connect(actionEVE, SIGNAL(triggered()), this, SLOT(EVEInit()));
    gameMenu->addAction(actionEVE);
    Init();
}

MainWindow::~MainWindow()
{
    if(Gobang){
        delete Gobang;
    }
}
void MainWindow::Init(){
    /*
    QPalette palette;
    QPixmap pixmap("./pic/background.jpg");
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);
    */
    this->setStyleSheet("background-image:url(D:/TOOLS/Five_In_Row/QFive_In_Row/Gobang/pic/bg6.jpg);");
    QIcon icon("./pic/circle.ico");
    this->setWindowIcon(icon);
    Gobang=new Gobang_Model;
    PVPInit();
}
void MainWindow::PVPInit(){
    this->setWindowTitle("五子棋（人人对战）");
    Gobang->gobang_type=PVP;
    Gobang->gobang_state=ING;
    Gobang->Start(Gobang->gobang_type);
    update();
}
void MainWindow::PVEInit(){
    this->setWindowTitle("五子棋（人机对战）");
    Gobang->gobang_type=PVE;
    Gobang->gobang_state=ING;
    Gobang->Start(Gobang->gobang_type);
    update();
}
void MainWindow::EVEInit(){
    this->setWindowTitle("五子棋（AI对战）");
    Gobang->gobang_type=EVE;
    Gobang->gobang_state=ING;
    Gobang->Start(Gobang->gobang_type);
    for(int i=1;i<boardsize;i++){
        for(int j=1;j<boardsize;j++){
            //Gobang->Start(Gobang->gobang_type);
            QScreen *screen = QGuiApplication::primaryScreen();
            AIRound();
            sleep(100);
            update();
            //Gobang->Map_Update(i,j);
            //update();
            sleep(500);
            QString q1="W"+QString::number(i)+"-"+QString::number(j);
            screen->grabWindow(this->winId()).save(q1,"PNG");
            sleep(500);
        }
    }
    /*
    QScreen *screen = QGuiApplication::primaryScreen();
//    screen->grabWindow(this->winId()).save("widget","png");
    Gobang->Map_Update(1,1);
    update();
    sleep(1000);
    screen->grabWindow(this->winId()).save("test1","png");
    Gobang->Map_Update(1,2);
    update();
    sleep(1000);
    screen->grabWindow(this->winId()).save("test2","png");
    */
    /*
    while(Gobang->gobang_state==ING&&Gobang->gobang_type==EVE){
        AIRound();
        sleep(100);
        update();
    }
    */
}

void MainWindow::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    //棋盘
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    for (int i = 0; i < boardsize + 1; i++)
    {
        painter.drawLine(BoardMargin + BlockSize * i, BoardMargin, BoardMargin + BlockSize * i, size().height() - BoardMargin);
        painter.drawLine(BoardMargin, BoardMargin + BlockSize * i, size().width() - BoardMargin, BoardMargin + BlockSize * i);

    }
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.drawEllipse(BoardMargin + BlockSize * 3 - 0.25 * Radius, BoardMargin + BlockSize * 3 - 0.25 * Radius, 0.5*Radius, 0.5*Radius);
    painter.drawEllipse(BoardMargin + BlockSize * 3 - 0.25 * Radius, BoardMargin + BlockSize * 11 - 0.25 * Radius, 0.5*Radius, 0.5*Radius);
    painter.drawEllipse(BoardMargin + BlockSize * 7 - 0.25 * Radius, BoardMargin + BlockSize * 7 - 0.25 * Radius, 0.5*Radius, 0.5*Radius);
    painter.drawEllipse(BoardMargin + BlockSize * 11 - 0.25 * Radius, BoardMargin + BlockSize * 3 - 0.25 * Radius, 0.5*Radius, 0.5*Radius);
    painter.drawEllipse(BoardMargin + BlockSize * 11 - 0.25 * Radius, BoardMargin + BlockSize * 11 - 0.25 * Radius, 0.5*Radius, 0.5*Radius);

    //落子标记
    if (MouseClickRow > 0 && MouseClickRow < boardsize &&
        MouseClickCol > 0 && MouseClickCol < boardsize &&
        Gobang->GobangMap[MouseClickRow][MouseClickCol]==0)
    {
        if (Gobang->player==Player1)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(BoardMargin + BlockSize * MouseClickCol - MarkSize / 2, BoardMargin + BlockSize * MouseClickRow - MarkSize / 2, MarkSize, MarkSize);
    }

    //棋子
    for (int i = 0; i < boardsize; i++)
        for (int j = 0; j < boardsize; j++)
        {
            if (Gobang->GobangMap[i][j] == 1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(BoardMargin + BlockSize * j - Radius, BoardMargin + BlockSize * i - Radius, Radius * 2, Radius * 2);
            }
            else if (Gobang->GobangMap[i][j] == -1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(BoardMargin + BlockSize * j - Radius, BoardMargin + BlockSize * i - Radius, Radius * 2, Radius * 2);
            }
        }

    //胜负
    if (MouseClickRow > 0 && MouseClickRow < boardsize &&
        MouseClickCol > 0 && MouseClickCol < boardsize &&
        (Gobang->GobangMap[MouseClickRow][MouseClickCol] == 1 ||
            Gobang->GobangMap[MouseClickRow][MouseClickCol] == -1))
    {
        if (Gobang->Victory(MouseClickRow,MouseClickCol)&&Gobang->gobang_state==ING)
        {
            _state=1;
            Gobang->gobang_state = WIN;
            QString str;
            if (Gobang->GobangMap[MouseClickRow][MouseClickCol] == 1)
                str = "黑棋";
            else if (Gobang->GobangMap[MouseClickRow][MouseClickCol] == -1)
                str = "白棋";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "提示", str + " 获胜!");

            //直接开始下一局
            if (btnValue == QMessageBox::Ok)
            {
                Gobang->Start(Gobang->gobang_type);
                Gobang->gobang_state=ING;
            }
        }
    }


    // 判断死局
    if (Gobang->peace())
    {
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "提示", "平局");
        if (btnValue == QMessageBox::Ok)
        {
            Gobang->Start(Gobang->gobang_type);
            Gobang->gobang_state=ING;
        }

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //落子标记
    int x = event->x();
    int y = event->y();

    //边缘不能落子
    if (x >= BoardMargin + BlockSize / 2 &&
            x < size().width() - BoardMargin &&
            y >= BoardMargin + BlockSize / 2 &&
            y < size().height()- BoardMargin)
    {
        //最近左上角的点
        int col = x / BlockSize;
        int row = y / BlockSize;
        int leftTopPosX = BoardMargin + BlockSize * col;
        int leftTopPosY = BoardMargin + BlockSize * row;

        //根据半径距离选最近的
        MouseClickRow = -1;
        MouseClickCol = -1;
        int len = 0;

        //确定在范围内的点
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < PosDelta)
        {
            MouseClickRow = row;
            MouseClickCol = col;
        }
        len = sqrt((x - leftTopPosX - BlockSize) * (x - leftTopPosX - BlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < PosDelta)
        {
            MouseClickRow = row;
            MouseClickCol = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - BlockSize) * (y - leftTopPosY - BlockSize));
        if (len < PosDelta)
        {
            MouseClickRow = row + 1;
            MouseClickCol = col;
        }
        len = sqrt((x - leftTopPosX - BlockSize) * (x - leftTopPosX - BlockSize) + (y - leftTopPosY - BlockSize) * (y - leftTopPosY - BlockSize));
        if (len < PosDelta)
        {
            MouseClickRow = row + 1;
            MouseClickCol = col + 1;
        }
    }

    // 存了坐标后也要重绘
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(Gobang->gobang_type==PVP||Gobang->player==Player1){
        PersonRound();
        sleep(100);
    }

        if (Gobang->gobang_type==PVE&&Gobang->player==Player2&&Gobang->gobang_state!=WIN)
        {

            AIRound();
            sleep(100);
        }



}
void MainWindow::PersonRound(){
    if(MouseClickRow!=-1&&MouseClickCol!=-1&&Gobang->GobangMap[MouseClickRow][MouseClickCol]==0){
        Gobang->Type_PVP(MouseClickRow,MouseClickCol);
        update();
    }
}
void MainWindow::AIRound(){
    Gobang->Type_PVE(MouseClickRow,MouseClickCol);
    update();
}

bool MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    return true;
}
