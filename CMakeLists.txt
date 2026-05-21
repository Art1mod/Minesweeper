cmake_minimum_required(VERSION 3.19)
project(Minesweeper LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Minesweeper
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Cell.h
    minesweeperboard.h minesweeperboard.cpp
    minesweeperbutton.h minesweeperbutton.cpp
    Config.h

)

target_link_libraries(Minesweeper
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS Minesweeper
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Minesweeper
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# Wrap the resource file compilation in quotes to handle directory spaces smoothly
if(WIN32)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif()
