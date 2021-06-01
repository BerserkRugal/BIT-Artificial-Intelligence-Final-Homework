#include<gobang_moves.h>

void train_data(const string &address){
    KNN A(0);
    A.train_data(address);
}

void get_the_move(Mat before,Mat after,int _k){
    tLabel abscissa,ordinate,color;
    tData test_Data_ab[2];//��¼���ӵ����˺�����
    tData test_Data_or[2];//��¼���ӵ�����������
    vector<int> loc_col;//��¼���ӵ�������λ������
    vector<int> loc_row;//��¼���ӵ�������λ������
    KNN knn(_k);
    Mat dst_loc=image_process(before-after);
    loc_col.clear();
    loc_row.clear();
    for(int m=0;m<dst_loc.cols;m++){//Ѱ�����������ı߽�㣨����
        for(int n=0;n<dst_loc.rows;n++){
            if(dst_loc.at<Vec3b>(n,m)[0]==255&&dst_loc.at<Vec3b>(n,m)[1]==255&&dst_loc.at<Vec3b>(n,m)[2]==255){
                loc_col.push_back(m);
            }
        }
    }
    for(int m=0;m<dst_loc.rows;m++){//Ѱ�����������ı߽�㣨����
        for(int n=0;n<dst_loc.cols;n++){
            if(dst_loc.at<Vec3b>(m,n)[0]==255&&dst_loc.at<Vec3b>(m,n)[1]==255&&dst_loc.at<Vec3b>(m,n)[2]==255){
                loc_row.push_back(m);
            }
        }
    }
    test_Data_ab[0]=(double)loc_col.front()/(dst_loc.cols)*CATENUM;
    test_Data_ab[1]=(double)loc_col.back()/(dst_loc.cols)*CATENUM;
    test_Data_or[0]=(double)loc_row.front()/(dst_loc.rows)*CATENUM;
    test_Data_or[1]=(double)loc_row.back()/(dst_loc.rows)*CATENUM;
    cout<<"Matching horizontal line..."<<endl;
    knn.set_test_data(test_Data_ab);
    knn.get_all_distance();
    abscissa=knn.get_max_freq_label();
    cout<<endl;
    cout<<"Matching vertical line..."<<endl;
    knn.set_test_data(test_Data_or);
    knn.get_all_distance();
    ordinate=knn.get_max_freq_label();

    if(dst_loc.at<Vec3b>((double)(loc_row.front()+loc_row.back())/2,(double)(loc_col.front()+loc_col.back())/2)[0]==255
            &&dst_loc.at<Vec3b>((double)(loc_row.front()+loc_row.back())/2,(double)(loc_col.front()+loc_col.back())/2)[1]==255
            &&dst_loc.at<Vec3b>((double)(loc_row.front()+loc_row.back())/2,(double)(loc_col.front()+loc_col.back())/2)[2]==255)
        color=-1;
    else
        color=1;

    cout<<endl;
    if(color==-1)
        cout<<"Black's turn. ";
    else
        cout<<"White's turn. ";
    cout<<"MOVES: "<<char(abscissa+1+64)<<ordinate+1<<endl;
    show_board(ordinate,abscissa,color);
}

void show_board(int x,int y,int color){
    int board[16][16];
    memset(board,0,sizeof(board));
    board[x+1][y+1]=color;//��Ϊ�����ñ�ǩʱ���о��Ǵ��㿪ʼ����ʵ���������������϶���Ϊ1-15���������������ΪA-O������Ҫ��1
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
                        cout << (i == 1 ? "�Щ�" : i == (15) ? "�ة�" : "�੤");
                    else if (j == 1)
                        cout << (i == 1 ? "����" : i == (15) ? "����" : "����");
                    else if(j==(15))
                        cout << (i == 1 ? "��" : i == (15) ? "��" : "��");
                }
                else if (board[i][j] == -1)
                    cout << "��";
                else
                    cout << "��";
            }
            cout << endl;
        }

}
