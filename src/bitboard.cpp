#include "../include/bitboard.hpp"


Bitboard rotate_left(Bitboard bb, int s) { return (bb << s) | (bb >> (64-s)); }
Bitboard rotate_right(Bitboard bb, int s) { return (bb >> s) | (bb << (64-s)); }

Bitboard bb_flip_diag_A8_H1(Bitboard bb){
    Bitboard t;
    const Bitboard k1 = (Bitboard)0xaa00aa00aa00aa00;
    const Bitboard k2 = (Bitboard)0xcccc0000cccc0000;
    const Bitboard k4 = (Bitboard)0xf0f0f0f00f0f0f0f;
    t  = bb ^ (bb << 36);
    bb ^= k4 & (t ^ (bb >> 36));
    t  = k2 & (bb ^ (bb << 18));
    bb ^= t ^ (t >> 18);
    t = k1 & (bb ^ (bb << 9));
    bb ^= t ^ (t >> 9);
    return bb;
}

Bitboard bb_rotate_90_cw(Bitboard bb){
    return bb_flip_vertical(bb_flip_diag_A8_H1(bb));
}

Bitboard bb_rotate_90_ccw(Bitboard bb){
    return bb_flip_diag_A8_H1(bb_flip_vertical(bb));
}

Bitboard bb_rotate_45_cw(Bitboard bb){
    Bitboard k1 = 0xAAAAAAAAAAAAAAAA;
    Bitboard k2 = 0xCCCCCCCCCCCCCCCC;
    Bitboard k4 = 0xF0F0F0F0F0F0F0F0;
    bb ^= k1 & (bb ^ rotate_right(bb, 8));
    bb ^= k2 & (bb ^ rotate_right(bb, 16));
    bb ^= k4 & (bb ^ rotate_right(bb, 32));
    return bb;
}

Bitboard bb_rotate_45_ccw(Bitboard bb){
    Bitboard k1 = 0x5555555555555555;
    Bitboard k2 = 0x3333333333333333;
    Bitboard k4 = 0x0f0f0f0f0f0f0f0f;
    bb ^= k1 & (bb ^ rotate_right(bb, 8));
    bb ^= k2 & (bb ^ rotate_right(bb, 16));
    bb ^= k4 & (bb ^ rotate_right(bb, 32));
    return bb;
}

unsigned int bit_scan_delete(Bitboard& bb){
    unsigned int position = bit_scan(bb);
    bb &= bb - 1;
    return position;
}


void print_bb(Bitboard bitboard){
    for(int i=0;i<8;i++, std::cout << "\n")
        print_bits_of_byte((bitboard >> (56-i*8)) & 0xff);
}

void print_bits_of_byte(byte _byte){
    for(int i=0;i<8;i++){
        std::cout << (_byte & 1) << " ";
        _byte >>= 1;
    }
}

void print_bits_of_byte_msb(byte _byte){
    for(int i=0;i<8;i++){
        std::cout << (_byte & 0xff) << " ";
        _byte <<= 1;
    }
}



