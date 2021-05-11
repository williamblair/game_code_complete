CC = g++
CFLAGS = -std=c++11 -g `pkg-config --cflags tinyxml2`
INCDIRS =  -I./
LIBDIRS = 
LIBS = `pkg-config --libs tinyxml2`
SOURCES = main.cpp 						\
					xml.cpp							\
					ActorFactory.cpp 		\
					Actor.cpp						\
					AmmoPickup.cpp			\
					HealthPickup.cpp		
TARGET = main
all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(INCDIRS) $(LIBDIRS) $(LIBS)

