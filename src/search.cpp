#include "../include/search.hpp"


Move search_negamax_root(Board::Board &pos, int depth){
    int max = -Score::MATE;
    Moves moves = pos.generate_pl_moves();
    Move max_move;
    std::unordered_map<int, int> depth_map;
    while(!moves.empty()){
        Move move = moves.back();
        pos.make_move(move);
        moves.pop_back();
        int score = -search_negamax(pos, depth-1, depth_map, true);
        pos.unmake_move(move);
        if(score > max){
            max_move = move;
            max = score;
        }
    }
    for(auto &[key, val] : depth_map )
       std::cerr << key << "   " << val << "\n";

    return max_move;
}


int search_negamax(Board::Board &pos, int depth, std::unordered_map<int, int> &depth_map, bool is_first_ply){
    if(depth_map.find(depth) == depth_map.end())
        depth_map.emplace(depth, 1);
    else
        depth_map[depth]++;
	if(pos.is_attacking(pos.get_king_pos(NOT_COLOR_C(pos.color_to_move)), NOT_COLOR_C(pos.color_to_move), true)){
            return Score::MATE + (is_first_ply == false ? -1 : 0) ;
	}

	if(depth == 0){
        return evaluate(pos);
	}

    Moves moves = pos.generate_pl_moves();
    int max = -Score::MATE;

    while(!(moves).empty()){
        Move move = moves.back();
        pos.make_move(move);
        moves.pop_back();
        int score = -search_negamax(pos, depth-1, depth_map, false);
        pos.unmake_move(move);
        if(score > max)
            max = score;
    }

    return max;
}
