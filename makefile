CXX = g++
CXXFLAGS= -std=c++11 #-o main
LDLIBS = -lncurses -pthread -lboost_system

main : main.cpp gui.cpp tickets.cpp IP4.cpp system.cpp 
