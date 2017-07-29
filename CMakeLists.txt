cmake_minimum_required(VERSION 3.0)
project(Reversi)

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)

if( DOXYGEN_FOUND )

    # input and output files
    set(DOXY_DEF_CFG ${CMAKE_CURRENT_BINARY_DIR}/doxy_def.cfg)
    set(DOXY_PRJ_CFG ${CMAKE_CURRENT_BINARY_DIR}/doxy_prj.cfg)

    if( NOT EXISTS ${DOXY_DEF_CFG} )
        execute_process(COMMAND ${DOXYGEN_EXECUTABLE} -g ${DOXY_DEF_CFG})
    endif()

    # generate project config file
    configure_file(${DOXY_DEF_CFG} ${DOXY_PRJ_CFG} @ONLY)

    # append project config to it
    FILE(APPEND "${DOXY_PRJ_CFG}" "PROJECT_NAME=\"${PROJECT_NAME}\"\n")
    FILE(APPEND "${DOXY_PRJ_CFG}" "OUTPUT_DIRECTORY=\"${CMAKE_CURRENT_BINARY_DIR}/doxy_docs\"\n")
    FILE(APPEND "${DOXY_PRJ_CFG}" "EXTRACT_PRIVATE=\"YES\"\n")
    FILE(APPEND "${DOXY_PRJ_CFG}" "INPUT=\"${CMAKE_CURRENT_SOURCE_DIR}\" \"${CMAKE_CURRENT_SOURCE_DIR}/doc\"\n")
    FILE(APPEND "${DOXY_PRJ_CFG}" "QUIET=YES\n")
    FILE(APPEND "${DOXY_PRJ_CFG}" "WARNINGS=NO\n")
    FILE(APPEND "${DOXY_PRJ_CFG}" "WARN_IF_UNDOCUMENTED=NO\n")
    FILE(APPEND "${DOXY_PRJ_CFG}" "WARN_IF_DOC_ERROR=NO\n")
    FILE(APPEND "${DOXY_PRJ_CFG}" "WARN_NO_PARAMDOC=NO\n")
    FILE(APPEND "${DOXY_PRJ_CFG}" "GENERATE_LATEX=NO\n")

    FILE(APPEND "${DOXY_PRJ_CFG}" "IMAGE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/doc\"\n")

    message("Doxygen build started")

    add_custom_target(doxygen_html ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXY_PRJ_CFG}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating documentation..."
            VERBATIM)

else()
    message("doxygen is needed to generate the documentation")
endif()

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)

if( UNIX )
    set(PDCurses /mnt/0de4eb21-22e7-407b-8e3f-204d9db348d7/vfdsk/prj-n/PDCurses)
else()
    set(PDCurses e:/p2/PDCurses)
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
        CursesGrid.h
        CursesGrid.cpp
        FieldList.h)

add_executable(Reversi ${SOURCE_FILES})

if( UNIX )
    target_link_libraries(Reversi ${PDCurses}/sdl1/libpdcurses.a SDL pthread)
else()
    target_link_libraries(Reversi ${PDCurses}/win32/pdcurses.lib ${PDCurses}/win32/panel.lib)

    set_target_properties(Reversi PROPERTIES LINK_FLAGS /NODEFAULTLIB:LIBCMT)
endif()
