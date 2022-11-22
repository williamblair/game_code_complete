CC = g++
CFLAGS = -g -Wall -std=c++17 `pkg-config --cflags bullet`
INCDIRS = -I. -I../lib
LIBDIRS = 
LIBS = -lSDL2 -lSDL2main -lGLEW -lGLU -lGL -lstdc++ -ldl -pthread -lz \
    `pkg-config --libs bullet` -lassimp
SOURCES = $(wildcard ./*.cpp) $(wildcard ../lib/*.cpp)
OBJS = $(SOURCES:.cpp=.o)
TARGET = main

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(INCDIRS)

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBDIRS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)
