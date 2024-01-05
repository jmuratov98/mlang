CONFIG := debug
ifeq ($(config), release)
	CONFIG := release
endif

SRC		:= src
INCLUDE		:= include/mlang

TESTS		:= mlang

BIN		:= bin
BINOBJ		:= bin-obj

MLANG_SOURCE_FILES	:= $(shell find ./${SRC}/ -name *.cpp)
MLANG_HEADER_FILES	:= $(shell find ./${INCLUDE}/ -name *.h)
MLANG_OBJECT_FILES	:= $(addprefix $(BINOBJ)/, $(patsubst %.cpp, %.o, $(notdir $(MLANG_SOURCE_FILES))))

COMMON   	:= -Wall -std=C++17 -march=native # -DNDEBUG
CFLAGS   	:= $(CFLAGS) $(COMMON)
CC       	:= clang++
LD       	:= $(CC)
LDFLAGS  	:= $(LDFLAGS)  # -L/path/to/libs/
LDADD    	:=  # -lrt
INCLUDE  	:= -Isrc # -I../path/to/headers/
DEFS     	:=  # -DLINUX

TARGET := $(BIN)/mlang.exe
.PHONY : all

all: $(TARGET)

ifeq ($(CONFIG), debug)
CFLAGS += -O0 -g
DEFS += -DMLANG_DEBUG
endif
ifeq ($(CONFIG), release)
	RLSFLAGS := -O2
	RLSDEFS := -DMLANG_RELEASE
	CFLAGS += $(RLSFLAGS)
	CXXFLAGS += $(RLSFLAGS)
	DEFS += $(RLSDEFS)
endif

$(TARGET) : $(MLANG_OBJECT_FILES) 
	$(LD) $(LDFLAGS) -o $@ $^ $(LDADD)

$(BINOBJ)/%.o : $(SRC)/%.cpp $(MLANG_HEADER_FILES) init
	$(CC) $(DEFS) $(INCLUDE) $(CFLAGS) -c $< -o $@

.PHONY : init clean help
init:
	mkdir -p ./bin/ ./bin-obj/

clean:
	rm -rf ./bin/* ./bin-obj/*

help:
	@echo "usage make config=[config]"
	@echo ""
	@echo "config:"
	@echo "        debug:                Builds debug build (default)"
	@echo "        release:              Builds release build"
