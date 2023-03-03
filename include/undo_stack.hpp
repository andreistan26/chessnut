#ifndef UNDO_H
#define UNDO_H

#include "moves.hpp"
#include <cstring>
#include <stack>

struct IrreversibleData{
    //(WR/WL/BR/BL)
    int castle_rights[2][2];
    bool has_enp;
    Square enp_square;
    byte halfmove_clock;
    
    IrreversibleData();
    IrreversibleData(int castle_rights[2][2], bool has_enp, Square enp_square, byte halfmove_clock);
    std::string to_string(bool indent = true) const ;
    friend std::ostream& operator<<(std::ostream& os, const IrreversibleData &data);
};


class UndoStack : public std::stack<IrreversibleData>{
public:
    std::string to_string(bool indent = true) const;
    void clear();
}; 



#endif
