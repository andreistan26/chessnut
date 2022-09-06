#ifndef ENGINE_H
#define ENGINE_H

enum class EngineState{
    NOT_INIT, INIT, THINKING, AWAITING
};

extern EngineState engine_state;


#endif ENGINE_H
