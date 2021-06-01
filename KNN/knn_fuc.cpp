#include <knn_fuc.h>

double random(double start, double end)
{
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}


KNN::KNN(int k)
{
    if(k==0)//训练模式
        return;
    ifstream fin;
    this->k = k;
    fin.open("../Model/KNN_Model.txt");//默认模型地址
    if(!fin)
    {
        cout<<"can not open the file ../Model/KNN_Model.txt"<<endl;
        exit(1);
    }
    for(int i=0;i<rowLen;i++)
    {
        for(int j=0;j<colLen;j++)
        {
            fin>>dataSet[i][j];
        }
        fin>>labels[i];
        //cout<<i<<" "<<labels[i]<<endl;
    }
    fin.close();
}

KNN::~KNN()
{
    map_index_dis.clear();
    map_label_freq.clear();
}

void KNN::set_test_data(tData *d){
    for(int i=0;i<colLen;i++)
        testData[i]=d[i];
    map_index_dis.clear();
    map_label_freq.clear();
}

void KNN::model_save(double dpoint[rowLen][colLen],int ipoint[rowLen]){
    fstream dataFile;
    dataFile.open("../Model/KNN_Model.txt",ios::out);
    if(!dataFile)                //检查文件是否创建成功
    {
        cout<<"error open"<<endl;
        exit(0);
    }
    for(int i=0;i<rowLen;i++){
        for(int j=0;j<colLen;j++){
            dataFile<<dpoint[i][j]<<" ";
        }
        dataFile<<ipoint[i]<<"\n";
    }
    dataFile.close();
}

Mat image_process(Mat imgp){
    Mat pimg;
    threshold(imgp,pimg,5, 255, THRESH_BINARY);//二值化处理
    return pimg;
}

double KNN:: get_distance(tData *d1,tData *d2)
{
    double sum = 0;
    for(int i=0;i<colLen;i++)
    {
        sum += pow( (d1[i]-d2[i]) , 2 );
    }
    return sqrt(sum);
}

//
void KNN::train_data(const string &address){
    double dataset[rowLen][colLen];
    int labelset[rowLen];
    int data_count=0;
    string filename=address;
    int num=CATENUM;
    double random(double,double);
    srand(unsigned(time(0)));
    Mat img_board=imread(address+"/Origin_Board/Board.PNG");
    Mat img_train,dst;
    vector<int> loc_col;//记录棋子的列轮廓端位坐标
    vector<int> loc_row;//记录棋子行轮廓端位坐标
    //先计入白棋数据集
    for(int p=1;p<=num;p++){
        for(int q=1;q<=num;q++){
            img_train=imread(filename+"/White/W"+to_string(p)+"-"+to_string(q)+".PNG");//要求棋盘图片与落子图片为尺寸相同的正方形
            dst=image_process(img_board-img_train);//对落子后图像与棋盘初始化状态图像进行作差处理并二值化，使整个图像呈现出明显的黑白效果，凸显出棋子的轮廓
/*
            namedWindow(filename);
            imshow(filename,dst);
            waitKey(0);
*/
            loc_col.clear();
            loc_row.clear();
            for(int m=0;m<dst.cols;m++){//寻找棋子轮廓的边界点（横向）
                for(int n=0;n<dst.rows;n++){
                    if(dst.at<Vec3b>(n,m)[0]==255&&dst.at<Vec3b>(n,m)[1]==255&&dst.at<Vec3b>(n,m)[2]==255){
                        loc_col.push_back(m);
                    }
                }
            }
            for(int m=0;m<dst.rows;m++){//寻找棋子轮廓的边界点（纵向）
                for(int n=0;n<dst.cols;n++){
                    if(dst.at<Vec3b>(m,n)[0]==255&&dst.at<Vec3b>(m,n)[1]==255&&dst.at<Vec3b>(m,n)[2]==255){
                        loc_row.push_back(m);
                    }
                }
            }
            //存入数据集中，并添加随机扰动（其中，令寻找到的横向边界像素坐标除以图片像素列数再乘棋盘的列数，纵向同理，实现数据的归一化）
            dataset[data_count][0]=(double)loc_col.front()/(dst.cols)*num+double(random(-0.1,0.1));
            dataset[data_count][1]=(double)loc_col.back()/(dst.cols)*num+double(random(-0.1,0.1));
            labelset[data_count]=q;data_count++;
            dataset[data_count][0]=(double)loc_row.front()/(dst.rows)*num+double(random(-0.1,0.1));
            dataset[data_count][1]=(double)loc_row.back()/(dst.rows)*num+double(random(-0.1,0.1));
            labelset[data_count]=p;data_count++;
        }
    }
    //计入黑棋数据集
    for(int p=1;p<=num;p++){
        for(int q=1;q<=num;q++){
            img_train=imread(filename+"/Black/B"+to_string(p)+"-"+to_string(q)+".PNG");
            dst=image_process(img_board-img_train);
            loc_col.clear();
            loc_row.clear();
            for(int m=0;m<dst.cols;m++){
                for(int n=0;n<dst.rows;n++){
                    if(dst.at<Vec3b>(n,m)[0]==255&&dst.at<Vec3b>(n,m)[1]==255&&dst.at<Vec3b>(n,m)[2]==255){
                        loc_col.push_back(m);
                    }
                }
            }
            for(int m=0;m<dst.rows;m++){
                for(int n=0;n<dst.cols;n++){
                    if(dst.at<Vec3b>(m,n)[0]==255&&dst.at<Vec3b>(m,n)[1]==255&&dst.at<Vec3b>(m,n)[2]==255){
                        loc_row.push_back(m);
                    }
                }
            }
            dataset[data_count][0]=(double)loc_col.front()/(dst.cols)*num+double(random(-0.1,0.1));
            dataset[data_count][1]=(double)loc_col.back()/(dst.cols)*num+double(random(-0.1,0.1));
            labelset[data_count]=q;data_count++;
            dataset[data_count][0]=(double)loc_row.front()/(dst.rows)*num+double(random(-0.1,0.1));
            dataset[data_count][1]=(double)loc_row.back()/(dst.rows)*num+double(random(-0.1,0.1));
            labelset[data_count]=p;data_count++;
        }
    }
    model_save(dataset,labelset);
}

void KNN:: get_all_distance()
{
    double distance;
    int i;
    for(i=0;i<rowLen;i++)
    {
        distance = get_distance(dataSet[i],testData);
        //<key,value> => <i,distance>
        map_index_dis[i] = distance;
    }
    map<int,double>::const_iterator it = map_index_dis.begin();
    while(it!=map_index_dis.end())
    {
        //cout<<"key = "<<it->first<<" distance = "<<it->second<<endl;
        it++;
    }
}

/*
 *寻找测试数据最接近的标签
 */
tLabel KNN:: get_max_freq_label()
{
    //把map_index_dis转换为vec_index_dis形式
    vector<pair<int,double>> vec_index_dis( map_index_dis.begin(),map_index_dis.end() );
    //按距离从小到大排列vec_index_dis从而获得最为接近的数据
    sort(vec_index_dis.begin(),vec_index_dis.end(),Cmp());

    cout<<"Selecting K-NearestNeighbor..."<<endl;
    for(int i=0;i<k;i++)
    {
        cout<<"The distance from"<<" the coordinate ("<<dataSet[ vec_index_dis[i].first ][0]<<","<<dataSet[ vec_index_dis[i].first ][1]<<")"<<" labeled " <<labels[vec_index_dis[i].first] <<" is "<<vec_index_dis[i].second<<endl;
        //计算每个标签的数量
        map_label_freq[ labels[ vec_index_dis[i].first ]  ]++;
    }

    map<tLabel,int>::const_iterator map_it = map_label_freq.begin();
    tLabel label;
    int max_freq = 0;
    //找到出现频率最高的标签
    while( map_it != map_label_freq.end() )
    {
        if( map_it->second > max_freq )
        {
            max_freq = map_it->second;
            label = map_it->first;
        }
        map_it++;
    }
    cout<<"The test data belongs to the "<<label<<" label"<<endl;
    return label;
}
