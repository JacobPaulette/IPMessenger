CXX = g++
CXXFLAGS= -std=c++11 -I include
LDLIBS = -lncurses -pthread -lboost_system
main :main.cpp source/gui.cpp source/tickets.cpp source/IP4.cpp source/system.cpp 
