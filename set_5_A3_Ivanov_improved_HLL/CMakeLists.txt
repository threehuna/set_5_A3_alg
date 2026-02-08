cmake_minimum_required(VERSION 3.30)
project(set_5_A3_Ivanov_improved_HLL)

set(CMAKE_CXX_STANDARD 20)

add_executable(set_5_A3_Ivanov_improved_HLL main.cpp
        RandomStreamGen.cpp
        RandomStreamGen.hpp
        HashFuncGen.cpp
        HashFuncGen.hpp
        HyperLogLog.cpp
        HyperLogLog.hpp
        Checkpoints.cpp
        Checkpoints.hpp
        Experiment.cpp
        Experiment.hpp)
