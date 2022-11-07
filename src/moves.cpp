#include "../include/moves.hpp"

const char *move_type_to_string[7] = {"Move", "Capture", "Ep", "Castle", "Promotion", "Double Pawn Push", "Empty"};

Move::Move(){
    Move(Square::a1, Square::a1, MoveType::Empty);
}

Move::Move(const Square from_, const Square to_, const MoveType move_type_, const PieceTypes piece_, const PieceTypes captured_):
                from{from_}, to{to_}, move_type{move_type_}, piece{piece_}, captured{captured_} {}

Move::Move(const std::string& move_str){
    from = string_to_square(move_str.substr(0, 2));
    to = string_to_square(move_str.substr(2, 2));
    move_type = MoveType::Move;
    if(move_str.size() == 5)
        move_type = MoveType::Promotion;
}

bool Move::empty(){
    if(from == Square::a1 && to == Square::a1 && move_type == MoveType::Empty)
        return true;
    else 
        return false;
}

Move& Move::operator=(const std::string &move_str){
    from = string_to_square(move_str.substr(0, 2));
    to = string_to_square(move_str.substr(2, 2));
    if(move_str.size() == 5)
        move_type = MoveType::Promotion;
    return *this;
}

std::ostream& operator<<(std::ostream &os, const Move &move){
    os  << (char)('a' + int(move.from) % 8) << int(move.from) / 8 + 1 << " "
        << (char)('a' + int(move.to) % 8) << int(move.to) / 8 + 1 << " "
        << std::to_string(int(move.move_type)) << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, const Moves &moves){
    for(auto move : moves){
        os << move;
    }
    return os;
}

std::string Move::to_string_uci(){
    std::string str = "";
    str += (char)('a' + int(from) % 8); str += (char)('0' + int(from) / 8 + 1);
    str += (char)('a' + int(to) % 8); str += (char)('0' + int(to) / 8 + 1);
    if(move_type == MoveType::Promotion)
        str += 'q';
    return str;
}


