#include "gobang_model.h"

void Gobang_Model::Start(Gobang_Type type){
    gobang_type=type;
    GobangMap.clear();//初始化棋盘
    for(int i=0;i<=boardsize;i++){
        vector<int> line;
        for(int j=0;j<=boardsize;j++){
            line.push_back(0);
        }
        GobangMap.push_back(line);
    }
    player=Player1;
}

void Gobang_Model::Map_Update(int board_row, int board_col){
    if(board_row<0||board_col<0)
        return;
    if(player==Player1){
        GobangMap[board_row][board_col]=1;
        player=Player2;
    }else if(player==Player2){
        GobangMap[board_row][board_col]=-1;
        player=Player1;
    }else{
        GobangMap[board_row][board_col]=0;
    }


}

void Gobang_Model::Type_PVP(int board_row, int board_col){
    Map_Update(board_row,board_col);
}

void Gobang_Model::Type_PVE(int &cRow, int &cCol){
    if(this->isEmpty()){
        Map_Update(boardsize/2,boardsize/2);
    }
    GobangMap_Forecast.clear();
    GobangMap_Forecast.assign(GobangMap.begin(),GobangMap.end());
    for(int i=1;i<boardsize;i++){
        for(int j=1;j<boardsize;j++){
            if(GobangMap_Forecast[i][j]==0){
                if(player==Player2)
                GobangMap_Forecast[i][j]=-1;
                if(player==Player1)
                GobangMap_Forecast[i][j]=1;
                if(Victory_Forecast(i,j)){
                    cRow=i;
                    cCol=j;
                    Map_Update(i,j);
                    return;
                }
                GobangMap_Forecast[i][j]=0;
            }
        }
    }


    mcts_context<Gobang_Model> mc;
    mc.state=*this;
    mc.root.action=make_pair(-1,-1);
    mc.root.children.clear();
    mc.root.parent=NULL;
    mc.root.n=0;
    mc.root.p=0;
    mc.root.q=0;
    mc.root.w=0;
    node_t<Gobang_Model> *pur_location=mct_search(mc,5000,player);
//    cout<<pur_location->n<<endl;
    cRow=pur_location->action.first;
    cCol=pur_location->action.second;
    Map_Update(cRow,cCol);

}

bool Gobang_Model::Victory(int board_row, int board_col){
    for (int i = 0; i < 5; i++)
    {
        //左右
        if ((GobangMap[board_row][board_col]==1||GobangMap[board_row][board_col]==-1)&&
                board_col - i > 0 &&
            board_col - i + 4 < boardsize &&
            GobangMap[board_row][board_col - i] == GobangMap[board_row][board_col - i + 1] &&
            GobangMap[board_row][board_col - i] == GobangMap[board_row][board_col - i + 2] &&
            GobangMap[board_row][board_col - i] == GobangMap[board_row][board_col - i + 3] &&
            GobangMap[board_row][board_col - i] == GobangMap[board_row][board_col - i + 4])
            return true;
    }

    // 竖直
    for (int i = 0; i < 5; i++)
        {
            if ((GobangMap[board_row][board_col]==1||GobangMap[board_row][board_col]==-1)&&
                    board_row - i > 0 &&
                board_row - i + 4 < boardsize &&
                GobangMap[board_row - i][board_col] == GobangMap[board_row - i + 1][board_col] &&
                GobangMap[board_row - i][board_col] == GobangMap[board_row - i + 2][board_col] &&
                GobangMap[board_row - i][board_col] == GobangMap[board_row - i + 3][board_col] &&
                GobangMap[board_row - i][board_col] == GobangMap[board_row - i + 4][board_col])
                return true;
        }

        // 左斜
        for (int i = 0; i < 5; i++)
        {
            if ((GobangMap[board_row][board_col]==1||GobangMap[board_row][board_col]==-1)&&
                    board_row + i < boardsize &&
                board_row + i - 4 > 0 &&
                board_col - i > 0 &&
                board_col - i + 4 < boardsize &&
                GobangMap[board_row + i][board_col - i] == GobangMap[board_row + i - 1][board_col - i + 1] &&
                GobangMap[board_row + i][board_col - i] == GobangMap[board_row + i - 2][board_col - i + 2] &&
                GobangMap[board_row + i][board_col - i] == GobangMap[board_row + i - 3][board_col - i + 3] &&
                GobangMap[board_row + i][board_col - i] == GobangMap[board_row + i - 4][board_col - i + 4])
                return true;
        }

        // 右斜
        for (int i = 0; i < 5; i++)
        {
            if ((GobangMap[board_row][board_col]==1||GobangMap[board_row][board_col]==-1)&&
                    board_row - i > 0 &&
                board_row - i + 4 < boardsize &&
                board_col - i > 0 &&
                board_col - i + 4 < boardsize &&
                GobangMap[board_row - i][board_col - i] == GobangMap[board_row - i + 1][board_col - i + 1] &&
                GobangMap[board_row - i][board_col - i] == GobangMap[board_row - i + 2][board_col - i + 2] &&
                GobangMap[board_row - i][board_col - i] == GobangMap[board_row - i + 3][board_col - i + 3] &&
                GobangMap[board_row - i][board_col - i] == GobangMap[board_row - i + 4][board_col - i + 4])
                return true;
        }

    return false;
}

bool Gobang_Model::Victory_Forecast(int board_row, int board_col){

    for (int i = 0; i < 5; i++)
        {
            //左右
            if ((GobangMap_Forecast[board_row][board_col]==1||GobangMap_Forecast[board_row][board_col]==-1)&&
                    board_col - i > 0 &&
                board_col - i + 4 < boardsize &&
                GobangMap_Forecast[board_row][board_col - i] == GobangMap_Forecast[board_row][board_col - i + 1] &&
                GobangMap_Forecast[board_row][board_col - i] == GobangMap_Forecast[board_row][board_col - i + 2] &&
                GobangMap_Forecast[board_row][board_col - i] == GobangMap_Forecast[board_row][board_col - i + 3] &&
                GobangMap_Forecast[board_row][board_col - i] == GobangMap_Forecast[board_row][board_col - i + 4])
                return true;
        }

        // 竖直
        for (int i = 0; i < 5; i++)
            {
                if ((GobangMap_Forecast[board_row][board_col]==1||GobangMap_Forecast[board_row][board_col]==-1)&&
                        board_row - i > 0 &&
                    board_row - i + 4 < boardsize &&
                    GobangMap_Forecast[board_row - i][board_col] == GobangMap_Forecast[board_row - i + 1][board_col] &&
                    GobangMap_Forecast[board_row - i][board_col] == GobangMap_Forecast[board_row - i + 2][board_col] &&
                    GobangMap_Forecast[board_row - i][board_col] == GobangMap_Forecast[board_row - i + 3][board_col] &&
                    GobangMap_Forecast[board_row - i][board_col] == GobangMap_Forecast[board_row - i + 4][board_col])
                    return true;
            }

            // 左斜
            for (int i = 0; i < 5; i++)
            {
                if ((GobangMap_Forecast[board_row][board_col]==1||GobangMap_Forecast[board_row][board_col]==-1)&&
                        board_row + i < boardsize &&
                    board_row + i - 4 > 0 &&
                    board_col - i > 0 &&
                    board_col - i + 4 < boardsize &&
                    GobangMap_Forecast[board_row + i][board_col - i] == GobangMap_Forecast[board_row + i - 1][board_col - i + 1] &&
                    GobangMap_Forecast[board_row + i][board_col - i] == GobangMap_Forecast[board_row + i - 2][board_col - i + 2] &&
                    GobangMap_Forecast[board_row + i][board_col - i] == GobangMap_Forecast[board_row + i - 3][board_col - i + 3] &&
                    GobangMap_Forecast[board_row + i][board_col - i] == GobangMap_Forecast[board_row + i - 4][board_col - i + 4])
                    return true;
            }

            // 右斜
            for (int i = 0; i < 5; i++)
            {
                if ((GobangMap_Forecast[board_row][board_col]==1||GobangMap_Forecast[board_row][board_col]==-1)&&
                        board_row - i > 0 &&
                    board_row - i + 4 < boardsize &&
                    board_col - i > 0 &&
                    board_col - i + 4 < boardsize &&
                    GobangMap_Forecast[board_row - i][board_col - i] == GobangMap_Forecast[board_row - i + 1][board_col - i + 1] &&
                    GobangMap_Forecast[board_row - i][board_col - i] == GobangMap_Forecast[board_row - i + 2][board_col - i + 2] &&
                    GobangMap_Forecast[board_row - i][board_col - i] == GobangMap_Forecast[board_row - i + 3][board_col - i + 3] &&
                    GobangMap_Forecast[board_row - i][board_col - i] == GobangMap_Forecast[board_row - i + 4][board_col - i + 4])
                    return true;
            }
    return false;
}


bool Gobang_Model::peace(){
    //无子可落
    for (int i = 1; i < boardsize; i++)
        for (int j = 1; j < boardsize; j++)
        {
            if (!(GobangMap[i][j] == 1 || GobangMap[i][j] == -1))
                return false;
        }
    return true;
}

bool Gobang_Model::isEmpty(){
    for (int i = 1; i < boardsize; i++){
        for (int j = 1; j < boardsize; j++){
            if(GobangMap[i][j] == 1||GobangMap[i][j] == -1)
                return false;
        }
    }
    return true;
}


//以下开始为蒙特卡洛树搜索
Gobang_Model next(Gobang_Model s,const pair<int,int>& a){
    if(s.peace()){
        return s;
    }
    Gobang_Model r=s;
    r.Map_Update(a.first,a.second);
    return r;
}

vector<pair<int,int>> actions(const Gobang_Model& s){
    vector<pair<int,int>> moves;
    for(int i=1;i<boardsize;i++){
        for(int j=1;j<boardsize;j++){
            if(s.GobangMap[i][j]==0&&(s.GobangMap[i-1][j]==1||s.GobangMap[i-1][j]==-1||s.GobangMap[i+1][j]==1||s.GobangMap[i+1][j]==-1||s.GobangMap[i][j-1]==1
                    ||s.GobangMap[i][j-1]==-1||s.GobangMap[i][j+1]==1||s.GobangMap[i][j+1]==-1||s.GobangMap[i+1][j+1]==1||s.GobangMap[i+1][j+1]==-1
                    ||s.GobangMap[i-1][j+1]==1||s.GobangMap[i-1][j+1]==-1||s.GobangMap[i+1][j-1]==1||s.GobangMap[i+1][j-1]==-1||s.GobangMap[i-1][j-1]==1||s.GobangMap[i-1][j+1]==-1)){
                pair<int,int> pn;
                pn.first=i;
                pn.second=j;
                moves.push_back(pn);
            }
        }
    }
    std::random_shuffle(moves.begin(),moves.end());
//    cout<<moves.size()<<endl;
    return moves;
}

vector<pair<int,int>> actions_simulation(const Gobang_Model& s){
    vector<pair<int,int>> moves;
    for(int i=1;i<boardsize;i++){
        for(int j=1;j<boardsize;j++){
            if(s.GobangMap[i][j]==0){
                pair<int,int> pn;
                pn.first=i;
                pn.second=j;
                moves.push_back(pn);
            }
        }
    }
    std::random_shuffle(moves.begin(),moves.end());
    return moves;
}


int playout(const Gobang_Model& s,Player p){
    Gobang_Model g=s;
    /*for(int i=1;i<boardsize;i++)
        for(int j=1;j<boardsize;j++)
            cout<<g.GobangMap[i][j];*/
    while(!g.peace()){
        vector<pair<int,int>> moves;
        moves=actions(g);
        std::random_shuffle(moves.begin(),moves.end());
        vector<pair<int,int> > ::iterator iter;
        for(iter=moves.begin();!g.peace()&&iter!=moves.end();iter++){
            g.Map_Update((*iter).first,(*iter).second);
            if(g.Victory((*iter).first,(*iter).second)){
                if(g.player==p)
                    return 1;
                else
                    return -1;
            }

        }

    }
    return 0;
}
