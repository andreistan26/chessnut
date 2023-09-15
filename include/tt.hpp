#ifndef TT_H
#define TT_H
#include <moves.hpp>
#include <random>
//key       64 bits
extern uint64_t prns_pieces[6][2][64];
extern uint64_t prns_castle[4];
extern uint64_t prns_ep[8];
extern uint64_t prns_color;

enum class NodeType : uint8_t {
    EXACT, ALPHA, BETA, ILLEGAL, EMPTY
};

struct TT_Entry{
    uint64_t key;
    uint8_t depth;
    uint16_t value;
    NodeType type;
    Move move;
    TT_Entry();
    TT_Entry(uint64_t key, uint8_t depth, uint16_t value, NodeType type, Move move);
};

class ZobristHash{
public:
    void static init();
};

class TT{
    uint64_t table_size;
    TT_Entry *entries;
public:
    TT();
    uint64_t get_table_size();
    TT_Entry& operator[](const uint64_t &key);
};

extern TT t_table;

#endif
