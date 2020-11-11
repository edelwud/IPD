#pragma once

#include <string>

class Logger {
public:
    Logger(std::string filename);
    void log();
    void close();
};