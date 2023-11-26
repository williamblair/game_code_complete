CXX = g++
CC = gcc
CXXFLAGS = -g -Wall -std=c++17
CFLAGS = -g -Wall
INCDIRS = -I. -I../lib -I../lib/LuaPlus/lua53-luaplus/src -I/usr/include/bullet
LIBDIRS = 
LIBS = -lSDL2 -lSDL2main -lGLEW -lGLU -lGL -lstdc++ -ldl -pthread -lz -lassimp -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath
CPPSOURCES = $(wildcard ./*.cpp) $(wildcard ../lib/GCC4/*.cpp) $(wildcard ../lib/LuaPlus/*.cpp)
CSOURCES = $(wildcard ../lib/LuaPlus/*.c) $(wildcard ../lib/LuaPlus/lua53-luaplus/src/*.c)
OBJS = $(CPPSOURCES:.cpp=.o) $(CSOURCES:.c=.o)
TARGET = main

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCDIRS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(INCDIRS)

all: $(OBJS)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJS) $(INCDIRS) $(LIBDIRS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)
