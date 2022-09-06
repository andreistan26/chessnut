#ifndef COMM_H
#define COMM_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include "moves.hpp"

using std::string;

enum class CommsState{
    AWAIT_GUI, UCI_MODE
};

extern CommsState comms_state;

void comms_init();

void init_engine();
void sigint_handler();
void command_move(Move move);
bool process_input(std::string input);
#endif
