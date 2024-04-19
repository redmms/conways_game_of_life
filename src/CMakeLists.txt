cmake_minimum_required(VERSION 3.6)
project(console_game_of_life)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    mainwindow.h
    mainwindow.cpp
    board.h
    board.cpp
    local_types.h
    local_types.cpp
)

add_executable(console_game_of_life ${SOURCES})

set(SDL2_DIR "../${CMAKE_CXX_COMPILER_ID}/SDL2-2.30.2/cmake" CACHE STRING 
"In the second argument should be the way to your folder with SDL that contains sdl2-config.cmake")

find_package(SDL2 REQUIRED)

target_link_libraries(console_game_of_life PRIVATE ${SDL2_LIBRARIES})

get_target_property(SDL_SHARED_LIB SDL2::SDL2 IMPORTED_LOCATION)

file(COPY ${SDL_SHARED_LIB} DESTINATION ${CMAKE_BINARY_DIR})