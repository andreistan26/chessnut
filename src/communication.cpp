#include "../include/communication.hpp"

CommsState comms_state = CommsState::AWAIT_GUI;
Engine engine = Engine();

std::deque<std::string> commands_queue;
void comms_init(){
    std::cout.setf(std::ios::unitbuf);
    log_stream.setf(std::ios::unitbuf);
}

bool process_input(const std::string& input){
    std::string command;
    log_stream << "> " << input << '\n';
    if(input == "uci"){
        comms_state = CommsState::UCI_MODE;
        commands_queue.push_back(default_uci_name);
        commands_queue.push_back(default_uci_author);
        commands_queue.push_back("uciok\n");
    }else if(input == "isready"){
        command = "readyok\n";
    }else if(input == "ucinewgame"){
        
    }else if(input.find("position") == 0){
        engine.update_op_move(Move(last_word(input)), true);
    }else if(input.find("go") == 0){ 
        command = "bestmove ";
        Move move = engine.search_move();
        log_stream << move << std::endl;
        command += move.to_string_uci();
        command += '\n';
    }else if(input == "stop"){
        //TODO
    }else if(input == "ponderhit"){
        //TODO
    }else if(input == "quit"){
        exit(0);
    }else if(input.find("setoption") == 0){
        //TODO 
    }

    if(!command.empty())
        commands_queue.push_back(command);

    if(commands_queue.empty())
        return false;
    else
        return true;
}



void send_commands(){
    while(!commands_queue.empty()){
        std::cout << commands_queue.front();
        log_stream << "< " << commands_queue.front();
        commands_queue.pop_front();
    }
}

std::string last_word(const std::string &string){
    int last_ws = string.find_last_of(' ');
    std::string last_word = string.substr(last_ws+1);
    return last_word;
}
