#include<opencv2/opencv.hpp>
#include<bits/stdc++.h>
using namespace cv;
using namespace std;

void show_board(const int board[16][16]){
    cout << "  ";
        for (int i = 1; i < 16; i++) {
            cout << (char)(i + 64) << " ";
        }
        cout << endl;
        for (int i = 1; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                if (j == 0){
                    if(i<10)
                    cout <<" "<< i;
                    else
                        cout<<i;
                }
                else if (board[i][j] == 0) {
                    if (j > 1 && j < 15)
                        cout << (i == 1 ? "┬─" : i == (15) ? "┴─" : "┼─");
                    else if (j == 1)
                        cout << (i == 1 ? "┌─" : i == (15) ? "└─" : "├─");
                    else if(j==(15))
                        cout << (i == 1 ? "┐" : i == (15) ? "┘" : "┤");
                }
                else if (board[i][j] == 1)
                    cout << "●";
                else
                    cout << "○";
            }
            cout << endl;
        }

}

void fs_write(){
    int cnum=100;
    Mat data_whole,label_whole;
    //Mat dataArray[cnum],labelArray[cnum];
    vector<Mat>dataArray;
    vector<Mat>labelArray;
    for(int op=0;op<100;op++){

        string ad="../Renju_RIF_Dataset/70000Games/Board"+to_string(op)+".xml";
        FileStorage fs(ad, FileStorage::READ);
        vector<string> map;
        fs["move"]>>map;
        cout<<map.size()<<endl;
        fs.release();
        Mat data(225,map.size()-1,CV_32F,Scalar::all((0.)));
        Mat label(225,map.size()-1,CV_32F,Scalar::all((0.)));
        int b[16][16];
        memset(b,0,sizeof(b));
        for(int i=0;i<map.size();i++){
            char *tmp=(char*)map[i].c_str();
            int x=(int)tmp[0]-96;
            //cout<<x<<endl;
            string numby;
            if(map[i].length()==2)
                numby=map[i].substr(1,1);
            else if(map[i].length()==3)
                numby=map[i].substr(1,2);
            int y=atoi(numby.c_str());
            if(i%2==0)
                b[y][x]=1;
            else
                b[y][x]=-1;
            //show_board(b);
            int j=0;
            if(i<map.size()-1){
                for(int k=1;k<16;k++){
                    for(int r=1;r<16;r++){

                data.at<float>(j,i)=(float)(b[k][r]+1)/2;
                j++;
                    }
                }
            }
            if(i>0){
                label.at<float>((y-1)*15+x-1,i-1)=(y-1)*15+x-1;
                //label.at<float>((y-1)*15+x-1,i-1)=b[y][x];
                //cout<<x<<" "<<y<<endl;
            }

        }
        dataArray.push_back(data);
        labelArray.push_back(label);

    }
    hconcat(dataArray,data_whole);
    hconcat(labelArray,label_whole);
        /*
    FileStorage fs("../Renju_RIF_Dataset/Board0.xml", FileStorage::READ);
    vector<string> map;
    fs["move"]>>map;
    fs.release();
    Mat data(225,map.size()-1,CV_32F,Scalar::all((0.)));
    Mat label(225,map.size()-1,CV_32F,Scalar::all((0.)));
    int b[16][16];
    memset(b,0,sizeof(b));
    for(int i=0;i<map.size();i++){
        char *tmp=(char*)map[i].c_str();
        int x=(int)tmp[0]-96;
        //cout<<x<<endl;
        string numby;
        if(map[i].length()==2)
            numby=map[i].substr(1,1);
        else if(map[i].length()==3)
            numby=map[i].substr(1,2);
        int y=atoi(numby.c_str());
        if(i%2==0)
            b[y][x]=1;
        else
            b[y][x]=-1;
        show_board(b);
        int j=0;
        if(i<map.size()-1){
            for(int k=1;k<16;k++){
                for(int r=1;r<16;r++){
                    if(b[k][r]!=0)
                        cout<<k<<" "<<r<<endl;
            data.at<float>(j,i)=b[k][r];
            j++;
                }
            }
        }
        if(i>0){
            label.at<float>((y-1)*15+x-1,i-1)=(y-1)*15+x-1;
            //label.at<float>((y-1)*15+x-1,i-1)=b[y][x];
            cout<<x<<" "<<y<<endl;
        }
            //cout<<label<<endl;
        //cout<<y<<endl;

    }
    */
    FileStorage fs2("input_label.xml",FileStorage::WRITE);
    fs2<<"input"<<data_whole;
    fs2<<"target"<<label_whole;
    fs2.release();
}

void fl_write(){
    fstream dataFile;
        dataFile.open("Board2.xml",ios::out);
        if(!dataFile)                //检查文件是否创建成功
        {
            cout<<"error open"<<endl;
            exit(0);
        }
     dataFile<<"<?xml version=\"1.0\"?>"<<"\n";
     dataFile<<"<opencv_storage>"<<"\n";
     dataFile<<"<move>";
     dataFile<<"h8 h9 h6 i10 i6 i9 g9 g8 j11 i7 h10 i11 i8 k9 j10 h7 g7 j9 l9 k8 l7 g12 h11 e10 d11 f11 h13 h12 i12 k10 k7 e8 f7 e9 e11 c8";
     dataFile<<"</move>"<<"\n";
     dataFile<<"</opencv_storage>"<<"\n";
     dataFile.close();
}
void write_master(){
    int Begin, End;
        string X, Y;
        string Line;
        static char num = 0;
        ifstream inFile;
        ofstream dataFile;

        inFile.open("../Renju_RIF_Dataset/renjunet_v10_20201123.xml");	//打开文件用于读取数据。如果文件不存在，则打开出错。
        if (!inFile.is_open())			//判断文件是否成功打开
        {
            cout << "Error opening file" << endl;
            return;
        }
        int cnum=0;
        while (!inFile.eof())
        {
               getline(inFile, Line);                 //获取一行的数据，存放到Line中
            if ((Begin = Line.find("<move>")) != string::npos)//若该行中存在<move>的字符
            {
                string bname="Board"+to_string(cnum)+".xml";
                dataFile.open(bname);//打开文件用于写入数据
                dataFile<<"<?xml version=\"1.0\"?>"<<"\n";
                dataFile<<"<opencv_storage>"<<"\n";
                dataFile<<"<move>";
                Begin += 5;
                if (Begin == Line.find('>'))		//若下一个字符是'>'，则定位到了数据开始的位置
                {
                    End = Line.find('<', Begin + 1);//定位x坐标文本的结束下标
                    X = Line.substr(Begin + 1, (End - (Begin + 1)));//获取x坐标的文本信息
                    //argv[num][0] = atof(X.c_str()); //将字符类型转换为浮点类型存储
                    dataFile << X << "\n";//输出到文本

                }
                dataFile<<"</move>"<<"\n";
                dataFile<<"</opencv_storage>"<<"\n";
                dataFile.close();
                cnum++;
            }

        }
        inFile.close();
        //outFile.close();
}
int main(){
    fs_write();
        return 0;
}
