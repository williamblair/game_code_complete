CC = g++
CFLAGS = -g -Wall -std=c++11
INCDIRS = -I. -I../lib
LIBDIRS = 
LIBS = -lSDL2 -lSDL2main -lGLEW -lGLU -lGL -lstdc++ -ldl -pthread -lz
SOURCES = $(wildcard ./*.cpp) $(wildcard ../lib/*.cpp)
OBJS = $(SOURCES:.cpp=.o)
TARGET = 16_3dScenes

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(INCDIRS)

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBDIRS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)
