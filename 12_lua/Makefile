CC = g++
CFLAGS = -std=c++17 -g
LUAPLUS_DIR = /home/bj/luaplus
LUAPLUS_BUILD_DIR = $(LUAPLUS_DIR)/.build/bin.lua53-luaplus.gcc.linux64
INCDIRS = -I./ \
          -I$(LUAPLUS_DIR)/Src \
          -I$(LUAPLUS_DIR)/Src/LuaPlus/lua53-luaplus/src \
          -I../09_resource_cache \
          -I../08_zip \
          -I../07_coop_multitasker \
          -I../11_event_manager
LIBDIRS = -L$(LUAPLUS_BUILD_DIR)
LIBS = -llua53-luaplus-static -ldl -lz
OBJS = main.o \
       App.o \
       ScriptExports.o \
       LuaStateManager.o \
       ScriptProcess.o \
       ScriptEvent.o \
       ScriptEventListener.o \
       ScriptEventListenerMgr.o \
       EvtDataScriptEventTest.o \
       ../09_resource_cache/ResCache.o \
       ../09_resource_cache/ResHandle.o \
       ../09_resource_cache/Resource.o \
       ../09_resource_cache/WildcardMatch.o \
       ../09_resource_cache/ResourceZipFile.o \
       ../08_zip/ZipFile.o \
       ../07_coop_multitasker/ProcessManager.o \
       ../07_coop_multitasker/Process.o \
       ../11_event_manager/EventData.o \
       ../11_event_manager/EventManager.o
TARGET = main

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBDIRS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(INCDIRS)

clean:
	rm -f $(OBJS)

