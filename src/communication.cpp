#include "../include/communication.hpp"

CommsState comms_state = CommsState::AWAIT_GUI;

void comms_init(){
    std::cout.setf(std::ios::unitbuf);
}

bool process_input(std::string input){
    std::string command;
    if(input == "uci"){
        
    }else if(input == "isready"){
        //TODO check if precomputed are finalized
        command = "readyok\n";
    }else if(input == "ucinewgame"){

    }else if(input.find("position") == 0){
        //process position
    }else if(input == "go"){ 

    }else if(input == "stop"){

    }else if(input == "ponderhit"){

    }else if(input == "quit"){

    }else if(input.find("setoption") == 0){
        //process setoption
    }else{
        return false;
    }
    return true;
}
