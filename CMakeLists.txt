cmake_minimum_required(VERSION 3.13)

project(multi-armed-bandit)

set(CMAKE_CXX_STANDARD 17)

# set flags:
# -std=c++1z -- C++17
# -Wall -Wextra -Wpedantic -- enable warnings
# -Werror - warnings are fatal
# -O2 -- recommended optimization level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -Wpedantic -Werror -O2")

add_subdirectory(src)
