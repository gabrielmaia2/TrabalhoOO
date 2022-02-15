cmake_minimum_required(VERSION 3.8)
project(Game)

set(CMAKE_VERBOSE_MAKEFILE off)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(ALLEGRO_LIBS
        liballegro.so
        liballegro_acodec.so
        liballegro_audio.so
        liballegro_color.so
        liballegro_font.so
        liballegro_image.so
        liballegro_main.so
        liballegro_memfile.so
        liballegro_primitives.so
        liballegro_ttf.so
        )

set(FILES main.cpp display.cpp time.cpp geometry.cpp graphics.cpp program.cpp entity.cpp input.cpp scene.cpp map.cpp)
set(HEADERS display.h time.h vars.h geometry.h container.h graphics.h program.h entity.h input.h scene.h map.h)

add_executable(Game ${FILES} ${HEADERS})

include_directories(include)
include_directories(lib)
link_directories(lib)

target_link_libraries(Game ${ALLEGRO_LIBS})