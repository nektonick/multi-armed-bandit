#pragma once

#include <random> // random engine

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

} // namespace multiArmedBandit
