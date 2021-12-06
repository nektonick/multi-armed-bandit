set(APP console_test)

set(APP_HDR
    stdafx.h
    common_utils.h
    arms.h
    bandits.h
    )
source_group(h FILES ${APP_HDR})

set(APP_SRC
    stdafx.cpp
    common_utils.cpp
    arms.cpp
    bandits.cpp
    main.cpp
    )
source_group(cpp FILES ${APP_SRC})

add_executable(${APP} ${APP_SRC} ${APP_HDR})
