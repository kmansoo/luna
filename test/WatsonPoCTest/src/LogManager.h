#pragma once

#include "ccCore/ccSingleton.h"

#include <string>
#include <fstream>

// 2017.6.28, Mansoo(mansoo.kim@icloud.com), I'll implement it as soon as possible.

class LogManager : public Luna::ccSingleton<LogManager>
{
public:
    LogManager();
    ~LogManager();

public:
    void addLog(const std::string& tag, bool isReq, const std::string& log);

private:
    std::ofstream log_file_;
};

