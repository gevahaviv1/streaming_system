cmake_minimum_required(VERSION 3.23)
project(ex5_gevahaviv)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex5_gevahaviv
        Movie.cpp
        Movie.h
        RecommenderSystem.cpp
        RecommenderSystem.h
        RecommenderSystemLoader.cpp
        RecommenderSystemLoader.h
        RSUser.cpp
        RSUser.h
        RSUsersLoader.cpp
        RSUsersLoader.h
        presubmit.cpp)
