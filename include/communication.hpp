#ifndef COMM_H
#define COMM_H
#include <cstring>
#include <string>
#include <cassert>
#include <deque>
#include "movegen.hpp"
#include "engine.hpp"

const char default_uci_name[] = "id name Dony\n";
const char default_uci_author[] = "id author Dony_Author\n";
extern std::deque<std::string> commands_queue;

enum class CommsState{
    AWAIT_GUI, UCI_MODE
};

extern CommsState comms_state;

void comms_init();

void init_engine();
void sigint_handler();
void command_move(Move move);
bool process_input(const std::string &input);
std::string last_word(const std::string &string);
void send_commands();
#endif
