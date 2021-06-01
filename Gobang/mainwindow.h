#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QTime>
#include <math.h>
#include <QScreen>
#include <QGuiApplication>
#include <QMainWindow>
#include <gobang_model.h>

const int BoardMargin = 30; // 棋盘边缘空隙
const int Radius = 15; // 棋子半径
const int MarkSize = 6; // 落子标记边长
const int BlockSize = 40; // 格子的大小
const int PosDelta = 20; // 鼠标点击的模糊距离上限


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //绘制棋盘及棋子
    void paintEvent(QPaintEvent *event);
    //判断鼠标移动位置，模拟落点
    void mouseMoveEvent(QMouseEvent *event);
    //实际落点后棋局状态的切换
    void mouseReleaseEvent(QMouseEvent *event);
    bool sleep(unsigned int msec);//延时
private slots:
    void PersonRound();//人手回合
    void AIRound();//AI手回合
    void PVPInit();//初始化PVP模式
    void PVEInit();//初始化PVE模式
    void EVEInit();//初始化EVE模式
private:
    Gobang_Model *Gobang;
    //鼠标点击位置坐标
    int MouseClickCol=-1;
    int MouseClickRow=-1;

    int _state=0;//棋局状态指示符
    void Init();//基本初始化函数

};

#endif // MAINWINDOW_H
