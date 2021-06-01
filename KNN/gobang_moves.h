//Encoding with UTF8
#ifndef GOBANG_MOVES_H
#define GOBANG_MOVES_H

#endif // GOBANG_MOVES_H

#include<knn_fuc.h>

void train_data(const string &address);//训练数据集

void get_the_move(Mat before,Mat after,int _k);//获得落子颜色及位置

void show_board(int x,int y,int color);//在控制台加载棋盘
