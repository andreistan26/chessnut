#include "../include/undo_stack.hpp"

IrreversibleData::IrreversibleData() : castle_rights{{0, 0}, {0, 0}}, has_enp{false}, enp_square{Square::a1}, halfmove_clock{0} {}

std::string IrreversibleData::to_string(bool indent) const{
    std::string str = "";
    std::string indent_str = "";
    if(indent) indent_str = "\t";
    str += indent_str + "Castle rights(WR/WL/BR/BL) : ";

    for(int i = 0; i < 4; i++) 
        str += std::to_string(castle_rights[i/2][i%2]) + " "; 
    str += "\n";
    
    if(has_enp == true)   
        str +=  indent_str  + "En Passant square : " 
                        + (char)((int)'a' + int(enp_square) % 8)
                        + std::to_string(int(enp_square) / 8 + 1)
                        + "\n";
    
    str += indent_str + "Halfmove Clock " + std::to_string(halfmove_clock) + "\n"; 
    return str;
}

std::ostream& operator<<(std::ostream &os, const IrreversibleData &data){
    os << data.to_string(false);
    return os;
}

IrreversibleData::IrreversibleData(int castle_rights_[2][2], bool has_enp_, Square enp_square_, byte halfmove_clock_) : has_enp{has_enp_}, enp_square{enp_square_}, halfmove_clock{halfmove_clock_} {
    memcpy(&castle_rights, &castle_rights_, sizeof(castle_rights_));
}

std::string UndoStack::to_string(bool indent) const{
    std::string str = "";
    std::string indent_str = "";
    if(indent) indent_str = "\t";
    str += indent_str + "stack size : " + std::to_string(size()) + "\n";
    return str;
}