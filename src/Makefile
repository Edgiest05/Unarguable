.PHONY: all examples clean

HEADER_NAME		?= unarguable.h

SRC_PATH 		?= ../src
EXAMPLES_PATH 	?= ../examples

CC = gcc
CFLAGS = -Wall -Wextra -std=c90 -g3 -pedantic

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g3 -pedantic

INCLUDE = -I. -I$(SRC_PATH)

EXAMPLES_C_SOURCES = $(wildcard $(EXAMPLES_PATH)/*.c)
EXAMPLES_C_TARGETS = $(patsubst %.c,%_c.exe,$(EXAMPLES_C_SOURCES))

EXAMPLES_CXX_SOURCES = $(wildcard $(EXAMPLES_PATH)/*.cpp)
EXAMPLES_CXX_TARGETS = $(patsubst %.cpp,%_cpp.exe,$(EXAMPLES_CXX_SOURCES))

EXAMPLES_TARGETS = $(EXAMPLES_C_TARGETS) $(EXAMPLES_CXX_TARGETS)

all: examples

examples: $(EXAMPLES_TARGETS)

$(EXAMPLES_PATH)/%_c.exe: $(EXAMPLES_PATH)/%.c $(SRC_PATH)/$(HEADER_NAME) $(SRC_PATH)/Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $<

$(EXAMPLES_PATH)/%_cpp.exe: $(EXAMPLES_PATH)/%.cpp $(SRC_PATH)/$(HEADER_NAME) $(SRC_PATH)/Makefile
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $<

clean:
	-del /Q $(subst /,\,$(EXAMPLES_PATH))\*.exe 2>nul
