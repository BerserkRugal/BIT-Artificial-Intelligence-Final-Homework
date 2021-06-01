#ifndef GOBANG_MODEL_H
#define GOBANG_MODEL_H

#include<bits/stdc++.h>

using namespace std;
const int boardsize=14;//15x15的棋盘，边界不能下棋

enum Player{
    Player1,Player2
};

enum Gobang_Type{
    PVP,PVE,EVE
};

enum Gobang_State{
    ING,WIN,LOS
};



class Gobang_Model{
public:
    vector<vector<int>> GobangMap;//棋盘落子情况,白1，空0，黑-1.
    vector<vector<int>> GobangMap_Forecast;
    Player player;//当前执棋玩家
    Gobang_Type gobang_type;//当前对局类型
    Gobang_State gobang_state;//当前棋局状态
    void Start(Gobang_Type type);
    void Type_PVP(int board_row,int board_col);//人与人模式
    void Type_PVE(int &cRow, int &cCol);//人机模式
    void Map_Update(int board_row,int board_col);//更新棋盘
    bool Victory(int board_row,int board_col);//胜利条件
    bool Victory_Forecast(int board_row, int board_col);
    bool peace();//和棋
    bool isEmpty();//判断棋局是否为空
};

//定义节点
template<class _state>
struct node_t{
    node_t<_state> *parent;
    pair<int,int> action;
    double w;//得分
    double n;//节点访问次数
    double p;
    double q;
    vector<node_t<_state>> children;
};

//根节点，及其状态
template<class _state>
struct mcts_context{
    _state state;
    node_t<_state> root;
};

template<class _state>
node_t<_state>* mct_search(mcts_context<_state>& ctx,int dur,Player p){
    _state origstate=ctx.state;
    for(int i=0;i<dur;i++){
        node_t<_state> *leaf=select(ctx);
        if(leaf->n>0&&leaf->children.empty())
            leaf=expand(ctx,leaf);
        double score=playout(ctx.state,p);
        back_propagate(leaf,score);
        ctx.state=origstate;
    }
    return &*std::max_element(
        ctx.root.children.begin(), ctx.root.children.end(),
        [](const node_t<_state> a, const node_t<_state>& b)
        {
          return a.n > b.n;
        });

}

template<class _state>
node_t<_state>* select(mcts_context<_state>& ctx) {
    node_t<_state>* pNode=&ctx.root;
    while(pNode->n>0&&!pNode->children.empty()){
        pNode=&*max_element(pNode->children.begin(),pNode->children.end(),
                           [pNode](const node_t<_state>& a,const node_t<_state>& b){
            double x = a.p + std::sqrt(pNode->q / a.n);
            double y = b.p + std::sqrt(pNode->q / b.n);
            return x < y;
});
        ctx.state=next(ctx.state,pNode->action);
    }
    return pNode;
}

Gobang_Model next(Gobang_Model s,const pair<int,int>& a);


template<class _state>
node_t<_state>* expand(mcts_context<_state>& ctx, node_t<_state>* leaf) {

  std::vector<pair<int,int>> a = actions(ctx.state);
  if (a.empty()) return leaf;

  leaf->children.resize(a.size());

  std::transform(
    a.begin(),
    a.end(),
    leaf->children.begin(),
    [leaf](const pair<int,int>& act)
    {
      return node_t<_state> {leaf, act, 0, 0, 0, 0};
    }
  );

  leaf = &leaf->children.front();
  ctx.state = next(ctx.state, leaf->action);

  return leaf;
}

vector<pair<int,int>> actions(const Gobang_Model& s);
vector<pair<int,int>> actions_simulation(const Gobang_Model& s);

int playout(const Gobang_Model& s,Player p);

template<class _state>
void back_propagate(node_t<_state>* leaf, double score) {
  while (leaf) {
    if (score == 0) leaf->w += 0.1;
    else if (score == 1) leaf->w += 1;
    leaf->n += 1;
    leaf->p = leaf->w / leaf->n;
    leaf->q = std::log(leaf->n) * 2;

    leaf = leaf->parent;
    score = -score;
  }
}

#endif // GOBANG_MODEL_H
