cmake_minimum_required(VERSION 3.13)

project(multi-armed-bandit)

set(CMAKE_CXX_STANDARD 17)

# -std=c++1z - C++17
# -Wall - enable warnings
# -O2 - optimization level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -O2")

set(APP console_test)

set(APP_HDR
    stdafx.h
    arm.h
    bandit.h
)
source_group(Header Files ${APP_HDR})

set(APP_SRC
    stdafx.cpp
    main.cpp
)
source_group(Source Files ${APP_SRC})


add_executable(${APP} ${APP_SRC} ${APP_HDR})
