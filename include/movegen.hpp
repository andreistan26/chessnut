#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.hpp"
#include "moves.hpp"

#define get_up_left_att_bb(X) (((X) & 0xfefefefefefe) << 15)
#define get_up_right_att_bb(X) (((X) & 0x7f7f7f7f7f7f) << 17)
#define get_left_up_att_bb(X) (((X) & 0xfcfcfcfcfcfcfc) << 6)
#define get_right_up_att_bb(X) (((X) & 0x3f3f3f3f3f3f3f) << 10)
#define get_left_down_att_bb(X) (((X) & 0xfcfcfcfcfcfcfc00) >> 10)
#define get_right_down_att_bb(X) (((X) & 0x3f3f3f3f3f3f3f00) >> 6)
#define get_down_left_att_bb(X) (((X) & 0xfefefefefefe0000) >> 17)
#define get_down_right_att_bb(X) (((X) & 0x7f7f7f7f7f7f0000) >> 15)


#define BB_RANK_1 0xff
#define BB_RANK_8 0xff00000000000000
#define BB_RANK_2 0xff00
#define BB_RANK_3 0xff00 << 8
#define BB_RANK_7 0xff00ULL << 40
#define BB_RANK_6 0xff00ULL << 32
#define BB_FILE_A 0x101010101010101
#define BB_FILE_H 0x8080808080808080

constexpr unsigned char shift_45[15] = {0, 1, 3, 6, 10, 15, 21, 28, 36, 43, 49, 54, 58, 61, 63}; 


extern Bitboard knight_attacks[64];
extern Bitboard king_attacks[64]; 
extern Bitboard pawn_attacks[2][64];
extern Bitboard rank_attacks[64][256]; 
extern Bitboard file_attacks[64][256];
extern Bitboard first_diagonal_attacks[64][256];
extern Bitboard second_diagonal_attacks[64][256];

constexpr int diagonal_shift_mine[64] = {
    0 , 57, 50, 43, 36, 29, 22, 15,
    8 , 0 , 57, 50, 43, 36, 29, 22,
    16, 8 , 0 , 57, 50, 43, 36, 29,
    24, 16, 8 , 0 , 57, 50, 43, 36,
    32, 24, 16, 8 , 0 , 57, 50, 43, 
    40, 32, 24, 16, 8 , 0 , 57, 50, 
    48, 40, 32, 24, 16, 8 , 0 , 57,
    56, 48, 40, 32, 24, 16, 8 , 0
};

constexpr int diagonal_second_shift_mine[64] = {
    8 , 16, 24, 32, 40, 48, 56, 0 ,
    16, 24, 32, 40, 48, 56, 0 , 9 ,
    24, 32, 40, 48, 56, 0 , 9 , 18,
    32, 40, 48, 56, 0 , 9 , 18, 27,
    40, 48, 56, 0 , 9 , 18, 27, 36,
    48, 56, 0 , 9 , 18, 27, 36, 45,
    56, 0 , 9 , 18, 27, 36, 45, 54,
    0 , 9 , 18, 27, 36, 45, 54, 63
};

constexpr int diagonal_shift[64] = {
    0 , 56, 48, 40, 32, 24, 16, 8,
    9 , 0 , 56, 48, 40, 32, 24, 16,
    18, 9 , 0 , 56, 48, 40, 32, 24,
    27, 18, 9 , 0 , 56, 48, 40, 32,
    36, 27, 18, 9 , 0 , 56, 48, 40, 
    45, 36, 27, 18, 9 , 0 , 56, 48, 
    54, 45, 36, 27, 18, 9 , 0 , 56,
    63, 54, 45, 36, 27, 18, 9 , 0
};

constexpr int diagonal_id[64] = {
    8, 7, 6, 5, 4, 3, 2, 1,
    9, 8, 7, 6, 5, 4, 3, 2,
    10,9, 8, 7, 6, 5, 4, 3,
    11,10,9, 8, 7, 6, 5, 4,
    12,11,10,9, 8, 7, 6, 5,
    13,12,11,10,9, 8, 7, 6,
    14,13,12,11,10,9, 8, 7,
    15,14,13,12,11,10,9, 8
};

constexpr int diagonal_len[64] = {
    8, 7, 6, 5, 4, 3, 2, 1,
    7, 8, 7, 6, 5, 4, 3, 2,
    6, 7, 8, 7, 6, 5, 4, 3,
    5, 6, 7, 8, 7, 6, 5, 4,
    4, 5, 6, 7, 8, 7, 6, 5,
    3, 4, 5, 6, 7, 8, 7, 6,
    2, 3, 4, 5, 6, 7, 8, 7,
    1, 2, 3, 4, 5, 6, 7, 8
};

constexpr int diagonal_sec_len[64] = {
    1, 2, 3, 4, 5, 6, 7, 8,
    2, 3, 4, 5, 6, 7, 8, 7,
    3, 4, 5, 6, 7, 8, 7, 6,
    4, 5, 6, 7, 8, 7, 6, 5,
    5, 6, 7, 8, 7, 6, 5, 4,
    6, 7, 8, 7, 6, 5, 4, 3,
    7, 8, 7, 6, 5, 4, 3, 2,
    8, 7, 6, 5, 4, 3, 2, 1
};

constexpr Square mapping_45_cw[64] = {
   Square::a1, Square::b2, Square::c3, Square::d4, Square::e5, Square::f6, Square::g7, Square::h8,
   Square::a2, Square::b3, Square::c4, Square::d5, Square::e6, Square::f7, Square::g8, Square::h1,
   Square::a3, Square::b4, Square::c5, Square::d6, Square::e7, Square::f8, Square::g1, Square::h2,
   Square::a4, Square::b5, Square::c6, Square::d7, Square::e8, Square::f1, Square::g2, Square::h3,
   Square::a5, Square::b6, Square::c7, Square::d8, Square::e1, Square::f2, Square::g3, Square::h4,
   Square::a6, Square::b7, Square::c8, Square::d1, Square::e2, Square::f3, Square::g4, Square::h5,
   Square::a7, Square::b8, Square::c1, Square::d2, Square::e3, Square::f4, Square::g5, Square::h6,
   Square::a8, Square::b1, Square::c2, Square::d3, Square::e4, Square::f5, Square::g6, Square::h7
};



inline Bitboard get_rank_attack(Square sq, Bitboard occupied){
    return rank_attacks[int(sq)][(occupied >> (int(sq)/8*8)) & 0xff];
}

inline Bitboard get_file_attack(Square sq, Bitboard occupied_90){
    return file_attacks[int(sq)][(occupied_90 >> (int(sq)%8*8)) & 0xff];
}

inline Bitboard get_first_diagonal_attack(Square sq, Bitboard rotated_45_cw){
    return first_diagonal_attacks[int(sq)][(rotated_45_cw >> diagonal_shift_mine[int(sq)]) & 0xff];
}

inline Bitboard get_second_diagonal_attack(Square sq, Bitboard rotated_45_ccw){
    return second_diagonal_attacks[int(sq)][(rotated_45_ccw >> diagonal_second_shift_mine[int(sq)]) & 0xff];
}


void init_lu_rank_att();
void init_lu_file_att();
void init_lu_first_diag_att();
void init_lu_second_diag_att();

void init_lu_pawn_att();
void init_lu_king_att();
void init_lu_knight_att();
void init_all_attack_bitboards();


void insert_moves(Bitboard bb_actions, Square from, MoveType type, PieceTypes piece_type, Moves& moves);

void generate_pawn_actions(Bitboard bb_pawns, Color color, Bitboard occupied[3], Moves& moves);
void generate_knight_actions(Bitboard bb_knight, Color color, Bitboard occupied[3], Moves& moves);
void generate_king_actions(Bitboard bb_king, Color color, Bitboard occupied[3], Moves& moves);
void generate_rook_actions(Bitboard bb_rook, Bitboard rotated_90, Color color, Bitboard occupied[3], Moves& moves);
void generate_bishop_actions(Bitboard bb_bishop, Bitboard rotated_45_cw, Bitboard rotated_45_ccw, Color color, Bitboard occupied, Moves& moves);
void generate_queen_actions(Bitboard bb_queen, Bitboard rotated_90, Bitboard rotated_45_cw, Bitboard rotated_45_ccw, Color color, Bitboard occupied[3], Moves& moves);

Bitboard generate_pawn_attacks_bb(Bitboard bb_pawns, Color color);


Bitboard generate_pawn_moves_bb(Bitboard bb_pawns, Color color);

#endif
