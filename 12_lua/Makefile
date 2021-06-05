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
SOURCES = main.cpp \
          App.cpp \
          InternalScriptExports.cpp \
          LuaStateManager.cpp \
          ScriptProcess.cpp \
          ScriptEvent.cpp \
          ScriptEventListener.cpp \
          ScriptEventListenerMgr.cpp \
          EvtDataScriptEventTest.cpp \
          ../09_resource_cache/ResCache.cpp \
          ../09_resource_cache/ResHandle.cpp \
          ../09_resource_cache/Resource.cpp \
          ../09_resource_cache/WildcardMatch.cpp \
          ../09_resource_cache/ResourceZipFile.cpp \
          ../08_zip/ZipFile.cpp \
          ../07_coop_multitasker/ProcessManager.cpp \
          ../07_coop_multitasker/Process.cpp \
          ../11_event_manager/EventData.cpp \
          ../11_event_manager/EventManager.cpp
TARGET = main

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(INCDIRS) $(LIBDIRS) $(LIBS)

