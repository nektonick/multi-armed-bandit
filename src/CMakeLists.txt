set(APP console_test)

set(APP_HDR
    stdafx.h
    arm.h
    bandit.h
)
source_group(hdr ${APP_HDR})

set(APP_SRC
    stdafx.cpp
    main.cpp
)
source_group(cpp ${APP_SRC})

add_executable(${APP} ${APP_SRC} ${APP_HDR})
