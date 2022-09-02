#include "../include/movegen.hpp"

Bitboard knight_attacks[64];
Bitboard pawn_attacks[2][64];
Bitboard king_attacks[64];
Bitboard rank_attacks[64][256];
Bitboard file_attacks[64][256]; 
Bitboard first_diagonal_attacks[64][256];
Bitboard second_diagonal_attacks[64][256];


void init_lu_pawn_att(){
    Color color = Color::White;
    for(int k = 0; k < 2; k++, color = Color::Black){
        for(Bitboard bb = 1; bb != 0; bb <<= 1){   
            if((color == Color::White && bb <= 0x100) || (color == Color::Black && bb > 0x80000000000000))
                { pawn_attacks[int(color)][bit_scan(bb)] = 0;}
            if(bb & BB_FILE_A)
                if(color == Color::White)
                    pawn_attacks[int(color)][bit_scan(bb)] = bb << 9;
                else 
                    pawn_attacks[int(color)][bit_scan(bb)] = bb >> 7;
            else if(bb & BB_FILE_H)
                if(color == Color::White)
                    pawn_attacks[int(color)][bit_scan(bb)] = bb << 7;
                else 
                    pawn_attacks[int(color)][bit_scan(bb)] = bb >> 9;
            else{
                if(color == Color::White){
                    pawn_attacks[int(color)][bit_scan(bb)] = (bb << 7 | bb << 9);      
                }else{
                    pawn_attacks[int(color)][bit_scan(bb)] = (bb >> 7 | bb >> 9);      
                }
            }
            
        }
    }
}

void init_lu_knight_att(){
    for(Bitboard i = 0; i < 64; i++){
        Bitboard knight_bitboard = 1ULL<<i;
        knight_attacks[i] = (
                get_down_right_att_bb(knight_bitboard)+
                get_down_left_att_bb(knight_bitboard)+
                get_right_down_att_bb(knight_bitboard)+
                get_right_up_att_bb(knight_bitboard)+
                get_left_down_att_bb(knight_bitboard)+
                get_left_up_att_bb(knight_bitboard)+
                get_up_left_att_bb(knight_bitboard)+
                get_up_right_att_bb(knight_bitboard)
        );
    }
}

void init_lu_king_att(){
    for(int i = 0; i < 64; i++){
        Bitboard bb_king = 1ULL << i;
        king_attacks[i]  = ((bb_king & ~BB_FILE_H) << 1) | ((bb_king & ~BB_FILE_A) >> 1) | bb_king;
        king_attacks[i] |= ((king_attacks[i] & ~BB_RANK_8) << 8) | ((king_attacks[i] & ~BB_RANK_1) >> 8);
        king_attacks[i] &= ~bb_king;
    }
}

void init_lu_rank_att(){
    unsigned char index[] = {1, 2, 4, 8, 16, 32, 64, 128};
    for(int i = 0; i < 8; i++){
        for(int rank = 0; rank < 256; rank++){
            Bitboard attack = 0;
            if((rank & index[i]) != 0){
                for(int it_right = i+1; it_right < 8; it_right++){
                    attack |= index[it_right];
                    if((index[it_right] & rank) != 0)
                        break;
                }
                for(int it_left = i-1; it_left >= 0; it_left--){
                    attack |= index[it_left];
                    if((index[it_left] & rank) != 0)
                        break;
                }
                //for simpler access when generating moves
                for(int j=0; j < 8; j++)
                    rank_attacks[j*8 + i][rank] = attack << (j*8);
            }else{
                //for simpler access when generating moves
                for(int j=0; j < 8; j++)
                    rank_attacks[j*8 + i][rank] = 0;
            }
        }
    }
}

//getting file mask is file_attacks[square][rotated >> sq%8*8]
void init_lu_file_att(){
    for(int i=0;i<64;i++){
        for(int j=0;j<256;j++){
            int file = i%8;
            int rank = i/8;
            file_attacks[i][j] = bb_rotate_90_ccw(rank_attacks[7-rank][j]) << file;       
        }
    }
}

void init_lu_first_diag_att(){
    byte index[] = {1, 2, 4, 8, 16, 32, 64, 128};
    int diagonals[9][2] = {{0},{1, 15}, {2, 14}, {3, 13}, {4, 12},
                        {5, 11}, {6, 10}, {7, 9}, {8}};
    int mappings[16][8] = {{0}, {7}, {6, 15}, {5, 14, 23}, {4, 13 ,22 ,31}, {3, 12, 21, 30 ,39},
                    {2, 11, 20, 29, 38, 47}, {1, 10, 19, 28, 37, 46, 55}, {0, 9, 18, 27, 36, 45, 54, 63}, 
                    {8, 17, 26, 35, 44, 53, 62}, {16, 25, 34, 43, 52, 61}, {24, 33, 42, 51, 60}, {32, 41, 50 ,59}, {40, 49, 58},
                    {48, 57}, {56}};
    for(int i = 8; i >= 1;i--){ // size of diagonals
        for(int diag : diagonals[i]){ // for each diagonal of the {i} size
            if(diag!=0){
                int* cells = mappings[diag];
                int mask = (1 << i)-1;
                for(int j = 1; j<= mask; j++){ // for each possible byte that fits in diagonala
                    //check for each square and see if it fits 
                    for(int cell_it = 0; cell_it < i; cell_it++){ //for each diagonal cell
                        int cell_mask = 1 << cell_it;
                        if((cell_mask & j) != 0){ // we check if the cell is on a occupied bit
                            byte byte_diag = (rank_attacks[cell_it][j] & mask);
                            for(int it=0; it < i; it++){  // we loop again through the cells to map them
                                if((byte_diag & index[it])!=0)
                                    set_bit(first_diagonal_attacks[cells[cell_it]][j], cells[it]);
                            }
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < 64; i++){
        int len = 8 - abs(i/8-i%8);
        int mask = (1 << len)-1; 
        for(int j = 0; j < 256; j++){
            if(first_diagonal_attacks[i][j] != 0){
               for(int k = j; k < 256; k++){
                   if((k & mask) == j){
                        first_diagonal_attacks[i][k] = first_diagonal_attacks[i][j];  
                   }
               } 
            }
        }
    }
}

void init_lu_second_diagonal_att(){
    byte index[] = {1, 2, 4, 8, 16, 32, 64, 128};
    int diagonals[9][2] = {{0},{1, 15}, {2, 14}, {3, 13}, {4, 12},
                        {5, 11}, {6, 10}, {7, 9}, {8}};
    int mappings[16][8] = {{0}, {0}, {8, 1}, {16, 9, 2}, {24, 17, 10, 3}, {32, 25, 18, 11, 4}, {40, 33, 26, 19, 12, 5},
                    {48, 41, 34, 27, 20, 13, 6}, {56, 49, 42, 35, 28, 21, 14, 7}, {57, 50, 43, 36, 29, 22, 15}, 
                    {58, 51, 44, 37, 30, 23}, {59, 52, 45, 38, 31}, {60, 53, 46, 39}, {61, 54, 47},
                    {62, 55}, {63}};
    for(int i = 8; i >= 1;i--){ // size of diagonals
        for(int diag : diagonals[i]){ // for each diagonal of the {i} size
            if(diag!=0){
            int* cells = mappings[diag];
            int mask = (1 << i)-1;
            for(int j = 1; j<= mask; j++){ // for each possible byte that fits in diagonala
                //check for each square and see if it fits 
                for(int cell_it = 0; cell_it < i; cell_it++){ //for each diagonal cell
                    int cell_mask = 1 << cell_it;
                    if((cell_mask & j) != 0){ // we check if the cell is on a occupied bit
                        byte byte_diag = (rank_attacks[cell_it][j] & mask);
                        for(int it=0; it < i; it++){  // we loop again through the cells to map them
                            if((byte_diag & index[it])!=0)
                                set_bit(second_diagonal_attacks[cells[cell_it]][j], cells[it]);
                        }
                    }
                }
            }
            }
        }
    }
    
    for(int i = 0; i < 64; i++){
        int len = (1+i/8+i%8)-(i/8)*(1+i/8+i%8)%8*2;
        int mask = (1 << len)-1; 
        for(int j = 0; j < 256; j++){
            if(second_diagonal_attacks[i][j] != 0){
               for(int k = j; k < 256; k++){
                   if((k & mask) == j){
                        second_diagonal_attacks[i][k] = second_diagonal_attacks[i][j];  
                   }
               } 
            }
        }
    }

}

void init_all_attack_bitboards(){
    init_lu_pawn_att();
    init_lu_king_att();
    init_lu_knight_att();
    init_lu_rank_att();
    init_lu_file_att();
    init_lu_first_diag_att();
    init_lu_second_diagonal_att();
}

void insert_moves(Bitboard bb_actions, Square from, MoveType type, Moves& moves){
    while(bb_actions){
        auto action_position = bit_scan_delete(bb_actions);
        Square to = static_cast<Square>(action_position);
        moves.add_move({from, to, type});
    }
}

void generate_pawn_actions(Bitboard bb_pawns, Color color, Bitboard occupied[3], Moves& moves){
    Bitboard bb_push[2];
    if(color == Color::White){
        bb_push[0] = (bb_pawns << 8) & ~occupied[2];
        bb_push[1] = ((bb_push[0] & BB_RANK_3) << 8) & ~occupied[2];
        while(bb_push[0]){
            Square to = static_cast<Square>(bit_scan_delete(bb_push[0]));
            Square from = static_cast<Square>(int(to) - 8);
            moves.add_move({from, to, MoveType::PAWN_PUSH_SINGLE});
        }

        while(bb_push[1]){
            Square to = static_cast<Square>(bit_scan_delete(bb_push[1]));
            Square from = static_cast<Square>(int(to) - 16);
            moves.add_move({from, to, MoveType::PAWN_PUSH_DOUBLE});
        }
            
    }else{
        bb_push[0] = (bb_pawns >> 8) & ~occupied[2];
        bb_push[1] = ((bb_push[0] & BB_RANK_6) >> 8) & ~occupied[2];
        while(bb_push[0]){
            Square to = static_cast<Square>(bit_scan_delete(bb_push[0]));
            Square from = static_cast<Square>(int(to) + 8);
            moves.add_move({from, to, MoveType::PAWN_PUSH_SINGLE});
        }
        while(bb_push[1]){
            Square to = static_cast<Square>(bit_scan_delete(bb_push[1]));
            Square from = static_cast<Square>(int(to) + 16);
            moves.add_move({from, to, MoveType::PAWN_PUSH_DOUBLE});
        }
    }

    while(bb_pawns){
        unsigned int pawn_position = bit_scan_delete(bb_pawns);
        Square from = static_cast<Square>(pawn_position);
        Bitboard bb_attack = pawn_attacks[int(color)][pawn_position] & occupied[NOT_COLOR(color)];
        insert_moves(bb_attack, from, MoveType::PAWN_ATTACK, moves);
    }
}

void generate_knight_actions(Bitboard bb_knights, Color color, Bitboard occupied[3], Moves& moves){
    while(bb_knights){
        unsigned int knight_position = bit_scan_delete(bb_knights);
        Square from = static_cast<Square>(knight_position);
        
        Bitboard bb_attacks = knight_attacks[knight_position] & occupied[NOT_COLOR(color)]; 
        insert_moves(bb_attacks, from, MoveType::KNIGHT_ATTACK, moves);
        
        Bitboard bb_moves = knight_attacks[knight_position] & ~occupied[int(color)];
        insert_moves(bb_moves, from, MoveType::KNIGHT_MOVE, moves);
    }
}

void generate_king_actions(Bitboard bb_king, Color color, Bitboard occupied[3], Moves& moves){
    unsigned int king_position = bit_scan_delete(bb_king);
    Square from = static_cast<Square>(king_position);

    Bitboard bb_attacks = king_attacks[king_position] & occupied[NOT_COLOR(color)];
    insert_moves(bb_attacks, from, MoveType::KING_ATTACK, moves);

    Bitboard bb_moves = king_attacks[king_position] & ~occupied[2];
    insert_moves(bb_moves, from, MoveType::KING_MOVE, moves);
}

void generate_rook_actions(Bitboard bb_rooks, Bitboard rotated_90, Color color, Bitboard occupied[3], Moves& moves){
    while(bb_rooks){
        unsigned int rook_position = bit_scan_delete(bb_rooks);
        Square from = static_cast<Square>(rook_position);
        Bitboard attack = ((rank_attacks[int(from)][(occupied[2] >> (int(from)/8*8)) & 0xff]) |
        file_attacks[int(from)][(rotated_90 >> (int(from)%8*8)) & 0xff]) & (~occupied[int(color)]);       
        insert_moves(attack, from, MoveType::ROOK_MOVE, moves);
    }
}

void generate_bishop_actions(Bitboard bb_bishops, Bitboard rotated_45_cw,
        Bitboard rotated_45_ccw, Color color, Bitboard occupied, Moves& moves){
    while(bb_bishops){
        unsigned int bishop_position = bit_scan_delete(bb_bishops);
        Square from = static_cast<Square>(bishop_position);
        Bitboard attack = (first_diagonal_attacks[int(from)][(rotated_45_cw >> diagonal_shift_mine[int(from)]) & 0xff] | 
        second_diagonal_attacks[int(from)][(rotated_45_ccw >> diagonal_second_shift_mine[int(from)]) & 0xff]) & ~occupied;
        insert_moves(attack, from, MoveType::BISHOP_MOVE, moves);
    }
}

void generate_queen_actions(Bitboard bb_queen, Bitboard rotated_90, Bitboard rotated_45_cw, Bitboard rotated_45_ccw, Color color,
        Bitboard occupied[3], Moves& moves){
    while(bb_queen){
        unsigned int queen_position = bit_scan_delete(bb_queen);
        Square from = static_cast<Square>(queen_position);
        Bitboard attack = ((first_diagonal_attacks[int(from)][(rotated_45_cw >> diagonal_shift_mine[int(from)]) & 0xff]
        | second_diagonal_attacks[int(from)][(rotated_45_ccw >> diagonal_second_shift_mine[int(from)]) & 0xff])
        | ((rank_attacks[int(from)][(occupied[2] >> (int(from)/8*8)) & 0xff]) | file_attacks[int(from)][(rotated_90 >> (int(from)%8*8)) & 0xff]))
        & ~occupied[int(color)];
        insert_moves(attack, from, MoveType::QUEEN_MOVE, moves);
    }
}

Bitboard generate_pawn_attacks_bb(Bitboard bb_pawns, Color color) {
    Bitboard bb_pawns_att = 0;
    if(color == Color::White){
        bb_pawns_att |= (bb_pawns & (~BB_FILE_A)) << 7;
        bb_pawns_att |= (bb_pawns & (~BB_FILE_H)) << 9;
    }else{

        bb_pawns_att |= (bb_pawns & (~BB_FILE_A)) >> 7;
        bb_pawns_att |= (bb_pawns & (~BB_FILE_H)) >> 9;
    }
    return bb_pawns_att;
}

Bitboard generate_pawn_moves_bb(Bitboard bb_pawns, Color color){
        Bitboard bb_pawns_moves = 0;
    if(color == Color::White){
        bb_pawns_moves |= bb_pawns << 8 ;
        bb_pawns_moves |= (bb_pawns & BB_RANK_2) << 16;
    }else{
        bb_pawns_moves |= (bb_pawns & BB_RANK_7) >> 16;
        bb_pawns_moves |= bb_pawns >> 8 ;
    }
    return bb_pawns_moves;
}
