#include "../include/communication.hpp"
#include "../include/board.hpp"
#include "../include/engine.hpp"

using std::cin;
using std::cout;
using std::cerr;


int main(){
    comms_init();
    std::string input;
    Board::Board board;
    while(true){
        getline(cin, input);
        if(process_input(input)){
            send_command();
        }
    }
    return 0;
}
