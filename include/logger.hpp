#ifndef LOGG_H
#define LOGG_H
#include <fstream>
#include <string>

extern std::ofstream log_stream;
extern std::ofstream intern_log;
const std::string default_log_file("engine_logs_");
const std::string default_intern_log_file("intern_engine_logs_");
#endif
