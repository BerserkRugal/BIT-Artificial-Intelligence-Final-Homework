//Encoding with UTF8
/*默认棋盘为正方形，即横纵坐标采用相同方式判断
  边界不允许落子
  从左到右，标签1-13代表横行线的B-N，即第2条（左边界右侧第一条）到第14条（右边界左侧第一条）；
  从上到下，标签1-13代表纵行线的2-14，即第2条（上边界下侧第一条）到第14条（下边界上侧第一条）
*/

#ifndef KNN_FUC_H
#define KNN_FUC_H


#endif // KNN_FUC_H

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

#define CATENUM 13;//棋盘可落子的横竖点数

typedef int tLabel;
typedef double tData;

using namespace std;
using namespace cv;

const int colLen = 2;//数据集维数
const int rowLen = 676;//数据集大小

double random(double start, double end);//为数据添加[start,end]区间的随机扰动
Mat image_process(Mat imgp);//对图片数据集进行处理

class KNN
{
public:
    KNN(int k);//初始化、加载模型、并设定K值，当K为0时，为训练模式
    ~KNN();//
    void train_data(const string &address);//训练图像数据集
    void set_test_data(tData *d);//设置要测试的数据，判定数据属于哪个分类
    void get_all_distance();//将新数据的每个特征与每个样本数据集的对应特征逐一比较
    tLabel get_max_freq_label();//提取样本特征空间中最邻近的分类标签
    struct Cmp
    {
        bool operator() (const pair<int,double>& lhs,const pair<int,double>& rhs)
        {
             return lhs.second < rhs.second;
         }
     };

private:
    int k;
    map<int,double> map_index_dis;//记录距离
    map<tLabel,int> map_label_freq;//记录每一个类标签出现的频率
    tData dataSet[rowLen][colLen];//样本数据集
    tData testData[colLen];//测试数据集
    tLabel labels[rowLen];//标签数据集
    void model_save(double dpoint[rowLen][colLen],int ipoint[rowLen]);//保存模型
    double get_distance(tData *d1,tData *d2);//获取两数据特征空间的距离

};
