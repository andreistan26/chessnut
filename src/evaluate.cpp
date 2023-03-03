#include "evaluate.hpp"


int apply_pst(Bitboard bitboard, const int pst[64], const Color &color){
    int score = 0;
    if(color == Color::Black){
        while(bitboard != 0)
            score += pst[bit_scan_delete(bitboard)];
    }else{
        while(bitboard != 0)
            score += pst[mirror_sq(bit_scan_delete(bitboard))];
    }
    return score;
}

void Evaluator::update(const Move &move, const Color &color, bool unmake){
    int accumulate = 0;
    accumulate -= pst_values[int(move.piece)][int(move.from)];
    accumulate += pst_values[int(move.piece)][int(move.to)];
    if(move.move_type == MoveType::Capture){
        accumulate += material_w[int(move.captured)];
        accumulate -= material_w[int(move.captured)];
    }else if(move.move_type == MoveType::Promotion){
        accumulate += pst_values[int(PieceTypes::Queen)][int(move.to)];
        accumulate += material_w[int(PieceTypes::Queen)];
        accumulate += material_w[int(PieceTypes::Pawn)];
    }else if(move.move_type == MoveType::Castle){
        accumulate -= pst_values[int(move.piece)][int(castled_rook_by_king[move.to].first)];
        accumulate += pst_values[int(move.piece)][int(castled_rook_by_king[move.to].second)];
    }
    score *= nm_score_sign(color);
    int sign = unmake ? -1 : 1;
    score += accumulate * nm_score_sign(color) * sign;
}

Evaluator::Evaluator(int initial_score){
    score = initial_score;
}

Evaluator::Evaluator(){
    score = 0;
}

int Evaluator::get_score(){
    return score;
}


