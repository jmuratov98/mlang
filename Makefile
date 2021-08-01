SRC		:= src
INCLUDE	:= include/mlang

TESTS	:= mlang

BIN		:= bin
BINOBJ	:= bin-obj

MLANG_SOURCE_FILES	:= $(shell find ./${SRC}/ -name *.c)
MLANG_HEADER_FILES	:= $(shell find ./${INCLUDE}/ -name *.h)
MLANG_OBJECT_FILES	:= $(addprefix $(BINOBJ)/, $(patsubst %.c, %.o, $(notdir $(MLANG_SOURCE_FILES))))

COMMON   	:= -Wall -Wformat=2 -march=native # -DNDEBUG
CFLAGS   	:= $(CFLAGS) $(COMMON)
CXXFLAGS 	:= $(CXXFLAGS) $(COMMON)
CC       	:= gcc
CXX      	:= g++
LD       	:= $(CC)
LDFLAGS  	:= $(LDFLAGS)  # -L/path/to/libs/
LDADD    	:=  # -lrt
INCLUDE  	:= -Iinclude # -I../path/to/headers/
DEFS     	:=  # -DLINUX

TARGET := $(BIN)/mlang.exe
.PHONY : all

all: $(TARGET)

# {{{ for debugging
DBGFLAGS := -g
DBGDEFS := -DMLANG_DEBUG
debug : CFLAGS += $(DBGFLAGS)
debug : CXXFLAGS += $(DBGFLAGS)
debug : DEFS += $(DBGDEFS)
debug : all
.PHONY : debug
# }}}

# {{{ for release
RLSFLAGS := -O2
RLSDEFS := -DMLANG_RELEASE
release : CFLAGS += $(RLSFLAGS)
release : CXXFLAGS += $(RLSFLAGS)
release : DEFS += $(RLSDEFS)
release : all
.PHONY : release
# }}}

$(TARGET) : $(MLANG_OBJECT_FILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDADD)

$(BINOBJ)/%.o : $(SRC)/%.c $(MLANG_HEADER_FILES)
	$(CC) $(DEFS) $(INCLUDE) $(CXXFLAGS) -c $< -o $@

.PHONY : init clean
init:
	mkdir ./bin/ ./bin-obj/
clean :
	rm -rf ./bin/* ./bin-obj/*