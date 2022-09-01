#ifndef BITBOARD_H
#define BITBOARD_H
#include <smmintrin.h>
#include <iostream>
#define Bitboard unsigned long long int
#define byte unsigned char

#define NOT_COLOR(C) ((int(C) == 0)) ? (1) : (0) 

inline unsigned int pop_count(Bitboard bb){
    return _mm_popcnt_u64(bb);
}

inline unsigned int bit_scan(Bitboard bb){
    return __builtin_ffsll(bb) - 1;
}

inline Bitboard bb_flip_vertical(Bitboard bb){
    return __builtin_bswap64(bb);
}

Bitboard rotate_right(Bitboard bb, int s);
Bitboard rotate_left(Bitboard bb, int s);

unsigned int bit_scan_delete(Bitboard& bb);
    
Bitboard bb_flip_diag_A8_H1(Bitboard bb);

Bitboard bb_rotate_90_cw(Bitboard bb);
Bitboard bb_rotate_90_ccw(Bitboard bb);
Bitboard bb_rotate_45_cw(Bitboard bb);
Bitboard bb_rotate_45_ccw(Bitboard bb);


void print_bb(Bitboard bitboard);
void print_bits_of_byte(byte _byte);
void print_bits_of_byte_msb(byte _byte);


inline void set_bit(Bitboard& bitboard, int square){
    bitboard |= 1ULL << square;
}

inline void erase_bit(Bitboard& bitboard, int square){
    bitboard &= ~(1ULL << square);
}

enum class PieceTypes{
    Pawn,
    Bishop,
    Knight,
    Rook,
    Queen,
    King
};

enum class Color{
    White,
    Black
};

enum class Square{
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

#endif
