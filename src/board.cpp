#include "../include/board.hpp"
namespace Board{
    
    Color engine_color = Color::White;
    Color current_color = Color::Black;
    int half_move_counter = 0;
    int move_counter = 0;

    Board::Board(bool init_default_fen){
        if(init_default_fen)
            Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        else{
            memset(bb_piece_type, 0, sizeof(bb_piece_type));
            memset(bb_occ, 0, sizeof(bb_occ));
            bb_occ_rot = 0;
            bb_occ_rot_main_diag = 0;
            bb_occ_rot_second_diag = 0;
            irr_data = IrreversibleData();
        }
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
        color_to_move = *(++it) == 'w' ? Color::White : Color::Black;
        it += 2;
        
        while(*it != ' '){
            switch(*it)
            {
                case 'K':
                {
                    irr_data.castle_rights[int(Color::White)][CASTLE_RIGHT] = 1;
                    break;
                }
                case 'Q':
                {
                    irr_data.castle_rights[int(Color::White)][CASTLE_LEFT] = 1;
                    break;
                }
                case 'k':
                {
                    irr_data.castle_rights[int(Color::Black)][CASTLE_RIGHT] = 1;
                    break;
                }
                case 'q':
                {
                    irr_data.castle_rights[int(Color::Black)][CASTLE_LEFT] = 1;
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

        it++;
        if(*it == '-')
            irr_data.has_enp = false;
        else{
            irr_data.has_enp = true;
            irr_data.enp_square = static_cast<Square>((*it-'a') * 8 + (*(++it)-'0'));
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

    //TODO dont use the rotation functions, better set the bits by mapping
    void Board::update_occupied_boards(){
        bb_occ[2] = bb_occ[0] | bb_occ[1];
        bb_occ_rot_main_diag = bb_rotate_45_cw(bb_occ[2]);
        bb_occ_rot_second_diag = bb_rotate_45_ccw(bb_occ[2]);
        bb_occ_rot = bb_rotate_90_cw(bb_occ[2]);
    }

    void Board::put_piece(int sq, PieceTypes type, int color){
        set_bit(bb_piece_type[color][int(type)], sq);
        set_bit(bb_occ[color], sq);
    }

    void Board::erase_piece(int sq, PieceTypes type, int color){
        erase_bit(bb_piece_type[color][int(type)], sq);
        erase_bit(bb_occ[color], sq);
    }
    
    MoveType Board::get_type(Move &move){
        if(move.move_type == MoveType::Promotion || move.move_type == MoveType::Capture) return move.move_type;
        if(move.piece == PieceTypes::Pawn && abs(int(move.from) - int(move.to)) == 16){
            return MoveType::Double_Pawn_Push;
        }else if(move.piece == PieceTypes::King && (abs(int(move.from) - int(move.to)) > 1 && abs(int(move.from) - int(move.to)) < 5)){
            return MoveType::Castle;
        }
        return MoveType::Move;
    }

    void Board::make_castle_move(const Move &move){
        static std::unordered_map<Square, std::pair<Square, Square>> rook_map = {
            {Square::c1, {Square::a1, Square::d1}},
            {Square::g1, {Square::h1, Square::f1}},
            {Square::c8, {Square::a8, Square::d8}},
            {Square::g8, {Square::h8, Square::f8}}
        };
        put_piece(int(move.to), PieceTypes::King, int(color_to_move));
        erase_piece(int(move.from), PieceTypes::King, int(color_to_move));
        put_piece(int(rook_map[move.to].second), PieceTypes::Rook, int(color_to_move));
        erase_piece(int(rook_map[move.to].first), PieceTypes::Rook, int(color_to_move));
    }

    void Board::make_move(Move &move){
        undo_stack.push(irr_data);
        if(move.move_type == MoveType::Castle){
            make_castle_move(move);
        }else if(move.move_type == MoveType::Promotion){
            put_piece(int(move.to), PieceTypes::Queen, int(color_to_move));
            erase_piece(int(move.from), move.piece, int(color_to_move));
        }else{
            //if(move.move_type != MoveType::Capture)
            update_move_capture(move);
            if(move.move_type == MoveType::Capture)
                erase_piece(int(move.to), move.captured, NOT_COLOR(color_to_move));
            put_piece(int(move.to), move.piece, int(color_to_move));
            erase_piece(int(move.from), move.piece, int(color_to_move));
        }
        
        update_castle_rights(move);
        //update_en_passant(move);
        update_occupied_boards();
        color_to_move = NOT_COLOR_C(color_to_move);
    }

    void Board::update_move_capture(Move &move){
        if(((bb_occ[NOT_COLOR(color_to_move)] >> int(move.to)) & 1) == 1){
            move.move_type = MoveType::Capture;
            for(int type = 0; type < 5; type++){
                if(((bb_piece_type[NOT_COLOR(color_to_move)][type] >> int(move.to)) & 1) == 1){
                    move.captured = static_cast<PieceTypes>(type);
                    break;
                }
            }
            //assert(move.piece != PieceTypes::Empty);
        }
    }

    void Board::update_en_passant(const Move &move){

        if(move.move_type == MoveType::Double_Pawn_Push){
            irr_data.has_enp = true;
            irr_data.enp_square = static_cast<Square>(int(move.to) + (color_to_move == Color::White ? -8 : 8));
        }else{
            irr_data.has_enp = false;
        }
    }
             
    void Board::update_castle_rights(const Move &move){
         if(move.piece == PieceTypes::King || move.move_type == MoveType::Castle){
            irr_data.castle_rights[int(color_to_move)][CASTLE_RIGHT] = 0;
            irr_data.castle_rights[int(color_to_move)][CASTLE_LEFT] = 0;
        }else if(move.piece == PieceTypes::Rook){
            if(move.from == Square::a1)
                irr_data.castle_rights[0][CASTLE_LEFT] = 0;
            else if(move.from == Square::h1)
                irr_data.castle_rights[0][CASTLE_RIGHT] = 0;
            else if(move.from == Square::a8)
                irr_data.castle_rights[1][CASTLE_LEFT] = 0;
            else if(move.from == Square::h8)
                irr_data.castle_rights[1][CASTLE_RIGHT] = 0;
        }
    }


    void Board::unmake_move(Move &move){
        color_to_move = NOT_COLOR_C(color_to_move);
        memcpy(&irr_data, &undo_stack.top(), sizeof(irr_data));
        if(move.move_type == MoveType::Castle){
            if(color_to_move == Color::White){
                put_piece(int(Square::e1), PieceTypes::King, int(color_to_move));
                if(move.to == Square::g1){
                    erase_piece(int(Square::g1), PieceTypes::King, int(color_to_move));
                    put_piece(int(Square::h1), PieceTypes::Rook, int(color_to_move));
                    erase_piece(int(Square::f1), PieceTypes::Rook, int(color_to_move));
                }else if(move.to == Square::c1){
                    erase_piece(int(Square::c1), PieceTypes::King, int(color_to_move));
                    put_piece(int(Square::a1), PieceTypes::Rook, int(color_to_move));
                    erase_piece(int(Square::d1), PieceTypes::Rook, int(color_to_move));
                }else{
                    //exit(3);
                }
            }else{
                put_piece(int(Square::e8), PieceTypes::King, int(color_to_move));
                if(move.to == Square::g8){
                    erase_piece(int(Square::g8), PieceTypes::King, int(color_to_move));
                    put_piece(int(Square::h8), PieceTypes::Rook, int(color_to_move));
                    erase_piece(int(Square::f8), PieceTypes::Rook, int(color_to_move));
                }else if(move.to == Square::c8){
                    erase_piece(int(Square::c8), PieceTypes::King, int(color_to_move));
                    put_piece(int(Square::a8), PieceTypes::Rook, int(color_to_move));
                    erase_piece(int(Square::d8), PieceTypes::Rook, int(color_to_move));
                }else{
                    //exit(4);
                }
            }
        }else if(move.move_type == MoveType::Ep){
            erase_piece(int(move.to), PieceTypes::Pawn, int(color_to_move));
            int enemy_pawn_sq = int(move.to) + (color_to_move == Color::White ? -8 : 8);
            put_piece(int(enemy_pawn_sq), PieceTypes::Pawn, NOT_COLOR(color_to_move));
            put_piece(int(move.from), move.piece, int(color_to_move));
        }else if(move.move_type == MoveType::Promotion){
            erase_piece(int(move.to), PieceTypes::Queen, int(color_to_move));
            put_piece(int(move.from), move.piece, int(color_to_move));
        }else if(move.move_type == MoveType::Capture){
            erase_piece(int(move.to), move.piece, int(color_to_move));
            put_piece(int(move.from), move.piece, int(color_to_move));
            put_piece(int(move.to), move.captured, NOT_COLOR(color_to_move));
        }else{
            put_piece(int(move.from), move.piece, int(color_to_move));
            erase_piece(int(move.to), move.piece, int(color_to_move));
        }
        undo_stack.pop();
        update_occupied_boards();
    }



    Moves Board::generate_pl_moves(bool castles){
        Moves moves;
        bb_occ[int(color_to_move)] |= bb_piece_type[int(NOT_COLOR(color_to_move))][int(PieceTypes::King)];
        generate_king_actions(bb_piece_type[int(color_to_move)][int(PieceTypes::King)], color_to_move, bb_occ, moves);

        if(irr_data.has_enp){
            put_piece(int(irr_data.enp_square), PieceTypes::Pawn, NOT_COLOR(color_to_move));
            generate_pawn_actions(bb_piece_type[int(color_to_move)][int(PieceTypes::Pawn)], color_to_move, bb_occ, moves);
            erase_piece(int(irr_data.enp_square), PieceTypes::Pawn, NOT_COLOR(color_to_move));
            for(Move &move : moves)
                if(move.to == irr_data.enp_square)
                    move.move_type = MoveType::Ep;
        }
        else{
            generate_pawn_actions(bb_piece_type[int(color_to_move)][int(PieceTypes::Pawn)], color_to_move, bb_occ, moves);
        }

        generate_knight_actions(bb_piece_type[int(color_to_move)][int(PieceTypes::Knight)], color_to_move, bb_occ, moves);
        generate_rook_actions(bb_piece_type[int(color_to_move)][int(PieceTypes::Rook)], bb_occ_rot, color_to_move, bb_occ, moves);
        generate_bishop_actions(bb_piece_type[int(color_to_move)][int(PieceTypes::Bishop)], bb_occ_rot_main_diag, bb_occ_rot_second_diag, color_to_move, bb_occ[int(color_to_move)], moves);
        generate_queen_actions(bb_piece_type[int(color_to_move)][int(PieceTypes::Queen)], bb_occ_rot, bb_occ_rot_main_diag, bb_occ_rot_second_diag, color_to_move, bb_occ, moves);
        bb_occ[int(color_to_move)] &= ~bb_piece_type[int(NOT_COLOR(color_to_move))][int(PieceTypes::King)];
        
        generate_castle_moves(moves);
        return moves;
    }

    Moves Board::generate_captures(){
        Moves moves;
        for(Move move : generate_pl_moves(false)){
            update_move_capture(move);
            if(move.move_type == MoveType::Capture)
                moves.push_back(move);
        }
        return moves;
    }

    //TODO unit
    void Board::generate_castle_moves(Moves &moves){
        //possible to get rid of the checking of the final king square to get pseudo legal moves instead of legal
        if(color_to_move == Color::Black){
            if(irr_data.castle_rights[1][CASTLE_LEFT] == 1)
                if(is_empty(bb_occ[2], {Square::b8, Square::c8, Square::d8}) &&
                    !(is_attacking(Square::b8, color_to_move, false) &&
                    is_attacking(Square::c8, color_to_move, false) &&
                    is_attacking(Square::d8, color_to_move, false) &&
                    is_attacking(Square::e8, color_to_move, true)))
                    moves.push_back({Square::e8, Square::c8, MoveType::Castle});
            if(irr_data.castle_rights[1][CASTLE_RIGHT] == 1)
                if(is_empty(bb_occ[2], {Square::g8, Square::f8}) &&
                    !(is_attacking(Square::f8, color_to_move, false) &&
                    is_attacking(Square::g8, color_to_move, false) &&
                    is_attacking(Square::e8, color_to_move, true)))
                    moves.push_back({Square::e8, Square::g8, MoveType::Castle});
        }else{ 
            if(irr_data.castle_rights[0][CASTLE_LEFT] == 1)
                if(is_empty(bb_occ[2], {Square::b1, Square::c1, Square::d1}) && 
                    !(is_attacking(Square::b1, color_to_move, false) &&
                    is_attacking(Square::c1, color_to_move, false) && 
                    is_attacking(Square::d1, color_to_move, false) &&
                    is_attacking(Square::e1, color_to_move, true)))
                    moves.push_back({Square::e1, Square::c1, MoveType::Castle});
            if(irr_data.castle_rights[0][CASTLE_RIGHT] == 1)
                if(is_empty(bb_occ[2], {Square::g1, Square::f1}) &&
                    !(is_attacking(Square::f1, color_to_move, false) &&
                    is_attacking(Square::g1, color_to_move, false) &&
                    is_attacking(Square::e1, color_to_move, true)))
                    moves.push_back({Square::e1, Square::g1, MoveType::Castle});
        }
    }
    // if piece of square(sq) and of color(sq_color) is attacked by opp color
    bool Board::is_attacking(Square sq, Color sq_color, bool is_king){
        if(pawn_attacks[int(sq_color)][int(sq)] & bb_piece_type[int(NOT_COLOR(sq_color))][int(PieceTypes::Pawn)])
            {/*std::cerr << "pawn "*/;return true;}
        if(knight_attacks[int(sq)] & bb_piece_type[int(NOT_COLOR(sq_color))][int(PieceTypes::Knight)])
            {/*std::cerr << "pawn "*/;return true;}
        if((get_first_diagonal_attack(sq, bb_occ_rot_main_diag) | get_second_diagonal_attack(sq, bb_occ_rot_second_diag)) &
                (bb_piece_type[int(NOT_COLOR(sq_color))][int(PieceTypes::Bishop)] | bb_piece_type[int(NOT_COLOR(sq_color))][int(PieceTypes::Queen)]))
            {/*std::cerr << "pawn "*/;return true;}
        if((get_file_attack(sq, bb_occ_rot) | get_rank_attack(sq, bb_occ[2])) &
                (bb_piece_type[int(NOT_COLOR(sq_color))][int(PieceTypes::Rook)] | bb_piece_type[int(NOT_COLOR(sq_color))][int(PieceTypes::Queen)]))
            {/*std::cerr << "pawn "*/;return true;}
        if(!is_king && (king_attacks[int(sq)] & bb_piece_type[int(NOT_COLOR(sq_color))][int(PieceTypes::King)]))
            {/*std::cerr << "pawn "*/;return true;}
        return false;
    }

    PieceTypes Board::get_piece(Square sq){
        if((square_bb[int(sq)] & bb_occ[2]) == 0) 
            return PieceTypes::Empty;
        Color color = (bb_occ[0] & square_bb[int(sq)]) ? Color::White : Color::Black;  
        for(int piece = 0; piece < 6; piece++){
            if((bb_piece_type[int(color)][piece] & square_bb[int(sq)]) != 0)
                return static_cast<PieceTypes>(piece);
        }
    }

    Square Board::get_king_pos(Color color){
        return static_cast<Square>(bit_scan(bb_piece_type[int(color)][int(PieceTypes::King)]));
    }

    unsigned int Board::piece_count(const PieceTypes &piece, const Color &color){
        return pop_count(bb_piece_type[int(color)][int(piece)]);
    } 

    const char Board::print_square (int square) const{
        if((bb_occ[2] & square_bb[square]) != 0){
            Color color = (bb_occ[0] & square_bb[square]) ? Color::White : Color::Black;  
            for(int piece = 0; piece < 6; piece++){
                if((bb_piece_type[int(color)][piece] & square_bb[square]) != 0)
                    return piece_to_char[int(color)][piece];
            }
        }else{
            return '_';
        }
    }

    std::string Board::debug_info(int flags) const {
        std::string info_str = "";
        std::string indent_str = "\t";

        if(flags & INFO_BOARD){
            info_str += "[Board]\n";
            for(int square = 56; square >=0; square-= 8){
                info_str += indent_str;
                for(int it = square; it < square + 8; it++){
                    info_str += print_square(it);
                }
                info_str += "\n";
            }
        }
        if(flags & INFO_IRR_DATA){
            info_str += "[Board irreversible data]\n" + irr_data.to_string();
        }
        if(flags & INFO_UNDO_STACK){
            info_str += "[Undo Stack]\n"
                     +  undo_stack.to_string();
        }
        if(flags & INFO_COLOR_TO_MOVE){
            info_str += "[Color to move]\n"
                     + indent_str + (color_to_move == Color::White ? "white\n" : "black\n");
        }
        return info_str;
    }


    std::ostream& operator<<(std::ostream &os, const Board &board){
        os << board.debug_info(INFO_BOARD);
        return os;
    }

}
