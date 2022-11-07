#ifndef SEARCH_H
#define SEARCH_H
#include "evaluate.hpp"
Move search_negamax_root(Board::Board &pos, int depth);
int search_negamax(Board::Board &pos, int depth, std::unordered_map<int, int>&depth_map, bool is_first_ply);
#endif
