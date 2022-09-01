#include "../include/board.hpp"
namespace Board{
    
    Color engine_color = Color::White;
    Color current_color = Color::Black;
    bool castle[2][2] = {{0, 0}, {0, 0}};
    bool king_has_moved[2] = {0, 0};
    bool rook_has_moved[2][2] = {{0, 0}, {0, 0}};
    int half_move_counter = 0;
    int move_counter = 0;

    Board::Board(){
        memset(bb_piece_type, 0, sizeof(bb_piece_type));
        memset(bb_occ, 0, sizeof(bb_occ));
        memset(bb_occ_rot, 0, sizeof(bb_occ_rot));
        memset(bb_occ_rot_main_diag, 0, sizeof(bb_occ_rot_main_diag));
        memset(bb_occ_rot_second_diag, 0, sizeof(bb_occ_rot_second_diag));
    }

    Board::Board(const char* fen_string): Board::Board(){
        int bb_it = 56;
        char* it = (char *)fen_string;
        for(; *it !=' '; it++){
            if(*it == '/'){
                bb_it -= 16;
            }else if(tolower(*it) >= 'a' && tolower(*it) <= 'z'){
                char_parse(*it, bb_it);
                bb_it++;
            }else if(FEN_CHECK_EMPTY(*it) == 1){
                bb_it += (*it - '0');
            }
        }
        //extract color
        color_to_move = *(++it) == 'w' ? Color::White : Color::Black;
        it += 2;
        
        //extract castle rules
        while(*it != ' '){
            switch(*it)
            {
                case 'K':
                {
                    castle[0][1] = 1;
                    break;
                }
                case 'Q':
                {
                    castle[0][0] = 1;
                    break;
                }
                case 'k':
                {
                    castle[1][1] = 1;
                    break;
                }
                case 'q':
                {
                    castle[1][0] = 1;
                    break;
                }
                case '-':
                {
                    it++;
                    break;
                }
            }
            it++;
        }

        //extract en passant
        it++;
        if(*it == '-')
            has_enp = false;
        else{
            has_enp = true;
            enp_square = (*it-'a') * 8 + (*(++it)-'0');
        }

        //extract moves
        half_move_counter = *(it) - '0';
        it += 2;
        move_counter = *(it) - '0';      

        update_occupied_boards();
    }

    void Board::char_parse(char c, int& sq){
        int color = !isupper(c);
        c = tolower(c);
        PieceTypes type;
        switch(c){
            case 'p':
            {
                type = PieceTypes::Pawn; 
                break;
            }
            case 'b':
            {
                type = PieceTypes::Bishop;
                break;
            }
            case 'n':
            {
                type = PieceTypes::Knight;
                break;
            }
            case 'r':
            {
                type = PieceTypes::Rook;
                break;
            }
            case 'q':
            {
                type = PieceTypes::Queen;
                break;
            }
            case 'k':
            {
                type = PieceTypes::King;
                break;
            }
        }
        put_piece(sq, type, color);
    }

    void Board::update_occupied_boards(){
        bb_occ_rot[0] = bb_rotate_90_cw(bb_occ[0]);
        bb_occ_rot[1] = bb_rotate_90_cw(bb_occ[1]);
        bb_occ_rot_main_diag[0] = bb_rotate_45_cw(bb_occ[0]);
        bb_occ_rot_main_diag[1] = bb_rotate_45_cw(bb_occ[1]);
        bb_occ_rot_second_diag[0] = bb_rotate_45_ccw(bb_occ[0]);
        bb_occ_rot_second_diag[1] = bb_rotate_45_ccw(bb_occ[1]);
        bb_occ[2] = bb_occ[0] | bb_occ[1];
    }

    void Board::put_piece(int sq, PieceTypes type, int color){
        set_bit(bb_piece_type[color][int(type)], sq);
        set_bit(bb_occ[color], sq);
    }

    void Board::erase_piece(int sq, PieceTypes type, int color){
        erase_bit(bb_piece_type[color][int(type)], sq);
        erase_bit(bb_occ[color], sq);
    }

    void Board::make_move(Move move){

    }
}
