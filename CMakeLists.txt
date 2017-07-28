cmake_minimum_required(VERSION 3.0)
project(Reversi)


# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
###    if( CMAKE_BUILD_TYPE MATCHES "^[Rr]elease" )
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/./doc/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM )
#    endif()
else ()
    message("Doxygen need to be installed to generate the doxygen documentation")
endif ()

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
