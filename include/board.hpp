#ifndef BOARD_H
#define BOARD_H
#include "moves.hpp"
#include "bitboard.hpp"
#include "movegen.hpp"
#include <cstring>
#define FEN_CHECK_PIECE(C) ((((C) <='Z' && (C) >= 'A') || ((C) <= 'z' && (C) >= 'a')) ? 1 : 0)
#define FEN_CHECK_EMPTY(C) (((C) >= '1' && (C) <= '8') ? 1 : 0)
/*
     a b c d e f g h 
    8      Q K
    7
    6
    5                B
    4                W
    3                  
    2
    1      Q k
     a b c d e f g h
*/
namespace Board{
    extern Color engine_color;
    extern Color current_turn_color;

    extern bool castle[2][2];
    extern bool king_has_moved[2];
    extern bool rook_has_moved[2][2];
    extern int half_move_counter;
    extern int move_counter;
    struct Board{
        Bitboard bb_piece_type[2][6];
        Bitboard bb_occ[3];
        Bitboard bb_occ_rot;
        Bitboard bb_occ_rot_main_diag;
        Bitboard bb_occ_rot_second_diag;
        Color color_to_move;
        int enp_square;
        bool has_enp;
    
        Board(bool init_default_fen = false);
        Board(const char* fen_string);

        void update_occupied_boards();
        void char_parse(char c, int& sq);
        void make_move(Move move);
        void put_piece(int sq, PieceTypes type, int color);
        void erase_piece(int sq, PieceTypes type, int color);

        void generate_pl_moves();
    private:
        Moves moves;
    };
}
#endif
