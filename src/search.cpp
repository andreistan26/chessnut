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
        int score = -search_negamax(pos, depth-1, depth_map);
        pos.unmake_move(move);
        if(score > max && score != Score::ILLEGAL_MOVE){
            max_move = move;
            max = score;
        }
    }
    // for(auto &[key, val] : depth_map )
    //    std::cerr << key << "   " << val << "\n";

    return max_move;
}


int search_negamax(Board::Board &pos, int depth, std::unordered_map<int, int> &depth_map){
    if(depth_map.find(depth) == depth_map.end())
        depth_map.emplace(depth, 1);
    else
        depth_map[depth]++;
	
    if(pos.is_attacking(pos.get_king_pos(NOT_COLOR_C(pos.color_to_move)), NOT_COLOR_C(pos.color_to_move), true)){
            return ILLEGAL_MOVE;
	}

	if(depth == 0){
#ifndef ITERATIVE_EVAL
        return Board::evaluate(pos);
#else
        return pos.eval.get_score();
#endif
	}

    Moves moves = pos.generate_pl_moves();
    int max = -Score::MATE;

    while(!(moves).empty()){
        Move move = moves.back();
        pos.make_move(move);
        moves.pop_back();
        int score = -search_negamax(pos, depth-1, depth_map);
        pos.unmake_move(move);
        if(score > max && score != Score::ILLEGAL_MOVE)
            max = score;
    }

    return max;
}


Move search_ab_root(Board::Board &pos, int depth){
    Moves moves = pos.generate_pl_moves();
    Move max_move;
    std::unordered_map<int, int> depth_map;
    int alpha = -MATE;
    int beta = MATE;
    while(!moves.empty()){
        Move move = moves.back();
        pos.make_move(move);
        moves.pop_back();
        int score = -search_ab(pos, depth-1, -beta, -alpha);
        pos.unmake_move(move);
        if(score == ILLEGAL_MOVE)
            continue;
        if(score > alpha){
            alpha = score;
            max_move = move;
        }
    }
    return max_move;
}

int search_ab(Board::Board &pos, int depth, int alpha, int beta){
	if(pos.is_attacking(pos.get_king_pos(NOT_COLOR_C(pos.color_to_move)), NOT_COLOR_C(pos.color_to_move), true)){
            return Score::ILLEGAL_MOVE;
	}

    //TT_Entry &ref = t_table[pos.hash_key];

    // // empty 
    
    /*
    if(ref.key == pos.hash_key && ref.depth >= depth){
        int score;
        if      (ref.type == NodeType::ALPHA && ref.value <= alpha)
            score = alpha;
        else if (ref.type == NodeType::BETA && ref.value >= beta) 
            score = beta;
        else if (ref.type == NodeType::ILLEGAL)
            return Score::ILLEGAL_MOVE;
        return (int)score * nm_score_sign(pos.color_to_move);
    }
    */
    //ref.type = NodeType::ALPHA;
    //ref.key = pos.hash_key;
    //ref.depth = depth;
	
    if(depth == 0){
#ifndef ITERATIVE_EVAL
        int eval_score = Board::evaluate(pos);
#else
        int eval_score = pos.eval.get_score();
#endif
        //ref = TT_Entry(
           // pos.hash_key,
           // 0,
           // eval_score,
           // NodeType::EXACT,
          //  Move()
        //);
        return eval_score;
    }
    
    Moves moves = pos.generate_pl_moves();
    while(!(moves).empty()){
        Move move = moves.back();
        pos.make_move(move);
        moves.pop_back();
        int score = -search_ab(pos, depth-1, -beta, -alpha);
        pos.unmake_move(move);
        if(score == Score::ILLEGAL_MOVE){
            //if(ref.type == NodeType::EMPTY) {
                // ref.move = move;
                //ref.type = NodeType::ILLEGAL;
            //}
            continue;
        } if(score >= beta) {
            //ref.type = NodeType::BETA;
            //ref.move = move;
            //ref.value = score;
            return beta;
        } if(score > alpha) {
 //           ref.type = NodeType::EXACT;
   //         ref.move = move;
            alpha = score;
        }
    }
    //ref.type = NodeType::ALPHA;
    //ref.value = alpha;
    return alpha;
}

int quies(Board::Board &pos, int alpha, int beta){
	if(pos.is_attacking(pos.get_king_pos(NOT_COLOR_C(pos.color_to_move)), NOT_COLOR_C(pos.color_to_move), true)){
            return Score::ILLEGAL_MOVE;
	}
    return 0;
}
