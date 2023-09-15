#include <tt.hpp>

uint64_t prns_pieces[6][2][64];
uint64_t prns_castle[4];
uint64_t prns_ep[8];
uint64_t prns_color;

TT t_table;

TT_Entry::TT_Entry(){
    key = 0;
    type = NodeType::EMPTY;
    value = 0;
    depth = 0;
}
TT_Entry::TT_Entry(uint64_t key, uint8_t depth, uint16_t value, NodeType type, Move move){
    this->key = key;
    this->depth = depth;
    this->value = value;
    this->type = type;
    this->move = move;
}

TT::TT() : table_size{1 << 24}{
    entries = new TT_Entry[table_size]();
}

void ZobristHash::init(){
    std::mt19937_64 gen;
    gen.seed(69);
    for(auto piece = 0; piece < 6; piece++){
        for(auto color = 0; color < 2; color++){
            for(auto sq = 0; sq < 64; sq++){
                prns_pieces[piece][color][sq] = gen();
            }
        }
    }
    for(auto castle = 0; castle < 4; castle++)
        prns_castle[castle] = gen();
    
    for(auto ep = 0; ep < 8; ep++){
        prns_ep[ep] = gen();
    }
    
    prns_color = gen();
}

TT_Entry& TT::operator[](const uint64_t &key){
    return entries[key % table_size];
}

inline uint64_t TT::get_table_size(){
    return table_size;
}
