LUA = ../../ext/lua
OUT_DIR = ../../lib

PSPSDK = $(shell psp-config --pspsdk-path)
PSPDIR = $(shell psp-config --psp-prefix)

CFLAGS = -O2 -G0 -I$(LUA)/include -D__PSP__

LIBLUA_OBJS = \
	$(LUA)/src/lapi.o \
	$(LUA)/src/lcode.o \
	$(LUA)/src/ldebug.o \
	$(LUA)/src/ldo.o \
	$(LUA)/src/ldump.o \
	$(LUA)/src/lfunc.o \
	$(LUA)/src/lgc.o \
	$(LUA)/src/llex.o \
	$(LUA)/src/lmem.o \
	$(LUA)/src/lobject.o \
	$(LUA)/src/lopcodes.o \
	$(LUA)/src/lparser.o \
	$(LUA)/src/lstate.o \
	$(LUA)/src/lstring.o \
	$(LUA)/src/ltable.o \
	$(LUA)/src/ltm.o \
	$(LUA)/src/lundump.o \
	$(LUA)/src/lvm.o \
	$(LUA)/src/lzio.o \
	$(LUA)/src/lauxlib.o \
	$(LUA)/src/lbaselib.o \
	$(LUA)/src/ldblib.o \
	$(LUA)/src/liolib.o \
	$(LUA)/src/lmathlib.o \
	$(LUA)/src/loslib.o \
	$(LUA)/src/ltablib.o \
	$(LUA)/src/lstrlib.o \
	$(LUA)/src/loadlib.o \
	$(LUA)/src/linit.o

INCDIR = ./lua_psp $(LUA)
OBJS = $(LIBLUA_OBJS)
TARGET_LIB = liblua.a

include $(PSPSDK)/lib/build.mak

install: all
	@cp $(TARGET_LIB) $(OUT_DIR)