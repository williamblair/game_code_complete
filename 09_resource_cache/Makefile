CC = g++
CFLAGS = -std=c++17 -g
INCDIRS = -I./ -I../08_zip
LIBDIRS = 
LIBS = -lz
SOURCES = main.cpp               \
          Resource.cpp           \
          ResHandle.cpp          \
          ResCache.cpp           \
          WildcardMatch.cpp      \
          ../08_zip/ZipFile.cpp  \
          ResourceZipFile.cpp
TARGET = main

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(INCDIRS) $(LIBDIRS) $(LIBS)

