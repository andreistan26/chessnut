#ifndef SEARCH_H
#define SEARCH_H
#include "board.hpp"
Move search_negamax_root(Board::Board &pos, int depth);
int search_negamax(Board::Board &pos, int depth, std::unordered_map<int, int>&depth_map);
Move search_ab_root(Board::Board &pos, int depth);
int search_ab(Board::Board &pos, int depth, int alpha, int beta);
int quies(Board::Board &pos, int alpha, int beta);
#endif
