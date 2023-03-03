#include "../include/board.hpp"
#include "../include/communication.hpp"
#include "../include/engine.hpp"

using std::cerr;
using std::cin;
using std::cout;

int main() {
  comms_init();
  std::string input;
  Board::Board board;
  while (true) {
    getline(cin, input);
    if (!input.empty() && process_input(input)) {
      send_commands();
    }
  }
  return 0;
}
