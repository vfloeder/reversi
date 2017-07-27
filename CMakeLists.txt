cmake_minimum_required(VERSION 3.0)
project(Reversi)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)

if(UNIX)
set( PDCurses /mnt/0de4eb21-22e7-407b-8e3f-204d9db348d7/vfdsk/prj-n/PDCurses )
else()
set( PDCurses e:/p2/PDCurses )
endif()

include_directories(${PDCurses})

set(SOURCE_FILES
        main.cpp
        TerminalWindow.h
        TerminalWindow.cpp
        Pos_Vect.h
        Reversi.h
        Reversi.cpp
        FieldValue.h
        QuadraticBoard.h
        GameHandler.cpp
        GameHandler.h
        CursesGrid.h CursesGrid.cpp FieldList.h)

add_executable(Reversi ${SOURCE_FILES})

if(UNIX)
target_link_libraries(Reversi ${PDCurses}/sdl1/libpdcurses.a SDL pthread)
else()
target_link_libraries(Reversi ${PDCurses}/win32/pdcurses.lib ${PDCurses}/win32/panel.lib)

set_target_properties(Reversi PROPERTIES LINK_FLAGS  /NODEFAULTLIB:LIBCMT)
endif()
