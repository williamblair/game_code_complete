CC = g++
CFLAGS = -std=c++17 -g
INCDIRS = -I./ -I../09_resource_cache -I../08_zip
LIBDIRS = 
LIBS = -lz
OBJS = main.o                                    \
       WaveResourceLoader.o                      \
       SoundResourceExtraData.o                  \
       ../09_resource_cache/Resource.o           \
       ../09_resource_cache/ResHandle.o          \
       ../09_resource_cache/ResCache.o           \
       ../09_resource_cache/WildcardMatch.o      \
       ../08_zip/ZipFile.o                       \
       ../09_resource_cache/ResourceZipFile.o
TARGET = main

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBDIRS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(INCDIRS)

clean:
	rm -f $(OBJS) $(TARGET)

