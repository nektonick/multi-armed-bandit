cmake_minimum_required(VERSION 3.13)

project(multi-armed-bandit)

set(CMAKE_CXX_STANDARD 17)

# set flags:
# -std=c++1z -- C++17
# -Wall -Wextra -Wpedantic -- enable warnings
# -Werror - warnings are fatal
# -O2 -- recommended optimization level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ") #-Wall 

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding additional compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wextra -Wpedantic -Werror -O2")
endif(CMAKE_COMPILER_IS_GNUCXX)

add_subdirectory(src)
