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

# on ubuntu do "sudo apt install libsdl1.2-dev" to install SDL

if( UNIX )
    find_package(SDL REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 17)

## build pdcurses as an external project

if( UNIX )
    set(cursesSubdir "sdl1")
    set(makeCommand "make")
else()
    set(cursesSubdir "wincon")
    set(makeCommand nmake -f makefile.vc)
endif()

# Enable CMake module

include(ExternalProject)

# download and build pdcurses

ExternalProject_Add(
  PDCurses
  GIT_REPOSITORY https://github.com/wmcbrine/PDCurses.git
  GIT_TAG "master"
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  # Disable update and patch
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/PDCurses/pdcurses/${cursesSubdir}
  SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/PDCurses/pdcurses
  INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/PDCurses/bin
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${makeCommand}
  # Disable install
  INSTALL_COMMAND ""
)

# Get pdcurses source and binary dirs

ExternalProject_Get_Property(PDCurses source_dir binary_dir)

# Create a lib target to be used as a dependency

if( DEBUG )
    set( debugtag "d" )
endif()

if( UNIX )
    add_library(libpdcurses IMPORTED STATIC GLOBAL)
    add_dependencies(libpdcurses PDCurses)
else()
    add_library(pdcurses${debugtag} IMPORTED STATIC GLOBAL)
    add_dependencies(pdcurses${debugtag} PDCurses)
endif()

# Set library properties

if( UNIX )
    set_target_properties(libpdcurses PROPERTIES
      "IMPORTED_LOCATION" "${binary_dir}/pdcurses.a"
      "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
      )
else()
    set_target_properties(pdcurses${debugtag} PROPERTIES
      "IMPORTED_LOCATION" "${binary_dir}/pdcurses${debugtag}.lib"
      "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
      )
endif()

# add include dir
include_directories("${source_dir}")

if( UNIX )
    set( PDCurses_Libs "libpdcurses" )
else()
    set( PDCurses_Libs "pdcurses${debugtag}")
endif()

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
    target_link_libraries(Reversi ${PDCurses_Libs} SDL pthread)
else()
    target_link_libraries(Reversi ${PDCurses_Libs} )

    set_target_properties(Reversi PROPERTIES LINK_FLAGS /NODEFAULTLIB:LIBCMT)
endif()

