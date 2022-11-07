#ifndef BOARD_H
#define BOARD_H
#include "moves.hpp"
#include "types.hpp"
#include "bitboard.hpp"
#include "movegen.hpp"
#include "undo_stack.hpp"
#include <unordered_map>
#include "logger.hpp"
#include <cstring>
#define FEN_CHECK_PIECE(C) ((((C) <='Z' && (C) >= 'A') || ((C) <= 'z' && (C) >= 'a')) ? 1 : 0)
#define FEN_CHECK_EMPTY(C) (((C) >= '1' && (C) <= '8') ? 1 : 0)
#define CASTLE_LEFT 0
#define CASTLE_RIGHT 1


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
    

    extern int half_move_counter;
    extern int move_counter;

    enum INFO_BOARD_DEBUG_FLAGS: int{
        INFO_NONE           = 0x0,
        INFO_BOARD          = 0x1,
        INFO_IRR_DATA       = 0x2,
        INFO_UNDO_STACK     = 0x4,
        INFO_COLOR_TO_MOVE  = 0x8,
        INFO_ALL            = 0xF
    };
    
    //OPERATORS_FOR(INFO_BOARD_FLAGS)


    struct Board{
        Bitboard bb_piece_type[2][6];
        Bitboard bb_occ[3];
        Bitboard bb_occ_rot;
        Bitboard bb_occ_rot_main_diag;
        Bitboard bb_occ_rot_second_diag;
        Color color_to_move;
        IrreversibleData irr_data; 
        UndoStack undo_stack;
    
        Board(bool init_default_fen = false);
        Board(const char* fen_string);

        void char_parse(char c, int &sq);
        void make_move(Move &move);
        void unmake_move(Move &move);
        Moves generate_pl_moves();

        int evaluate();
        
        unsigned int piece_count(const PieceTypes &piece, const Color &color);

        void update_move_capture(Move &move);

        //test if searching trough generated moves vs making moves backwards
        //TODO define check detection as internal function
        bool is_attacking(Square sq, Color sq_color, bool is_king=true);
        Square get_king_pos(Color color);
        PieceTypes get_piece(Square sq);
        MoveType get_type(Move &move);
        void generate_castle_moves(Moves &moves);

        std::string debug_info(int flags = INFO_ALL) const ;
        const char print_square (int square) const;
        friend std::ostream& operator<<(std::ostream& , const Board &board);
    private:

        void update_occupied_boards();
        void unmake_castle_move(const Move &move);
        void put_piece(int sq, PieceTypes type, int color);
        void erase_piece(int sq, PieceTypes type, int color);
        void update_castle_rights(const Move &move);
        void make_castle_move(const Move &move);
        void update_en_passant(const Move &move);
    };
}
#endif
