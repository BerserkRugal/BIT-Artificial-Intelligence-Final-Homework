
#include<gobang_moves.h>

int main(){
    //train_data("../");
    Mat img_before=imread("../Test_Moves/Test3_Before.PNG");
    Mat img_after=imread("../Test_Moves/Test3_After.PNG");
    get_the_move(img_before,img_after,5);
    return 0;
}
