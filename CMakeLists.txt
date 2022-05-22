cmake_minimum_required(VERSION 3.23)
project(boost_http)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH C:/Library/boost-1.79.0/)

add_compile_definitions(_WIN32_WINNT=_WIN32_WINNT_WIN10)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options("/EHa") #enable exceptions
    #elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    #    target_link_libraries(${PROJECT_NAME} -static) #static build
endif()
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.79.0)

set (PROJ_SRC
        )

add_executable(${PROJECT_NAME} ${PROJ_SRC} main.cpp)

target_link_libraries(${PROJECT_NAME} Boost::headers)