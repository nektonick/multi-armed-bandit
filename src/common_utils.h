#pragma once

#include <ctime>
#include <random>

// OS specific ENDL macro
#ifndef ENDL
#    ifdef _WIN32
#        define ENDL "\r\n"
#    else
#        define ENDL "\n"
#    endif
#endif

// tab is 4 space symbols
#define TAB "    "

namespace multiArmedBandit {

static std::mt19937 randomEngine(std::time(0));
static std::uniform_real_distribution<double> realDistr(0, 1);

} // namespace multiArmedBandit
