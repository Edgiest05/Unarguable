.PHONY: all examples clean

HEADER_NAME		?= unarguable.h

SRC_PATH 		?= ../src
EXAMPLES_PATH 	?= ../examples

ifeq ($(OS),Windows_NT)
	EXT = .exe
else 
	EXT = .bin
endif

CC = gcc
CFLAGS = -Wall -Wextra -Wno-sign-compare -std=c90 -g3 -pedantic

CXX = g++
CXXFLAGS = -Wall -Wextra -Wno-sign-compare -std=c++11 -g3 -pedantic

INCLUDE = -I. -I$(SRC_PATH)

EXAMPLES_C_SOURCES = $(wildcard $(EXAMPLES_PATH)/*.c)
EXAMPLES_C_TARGETS = $(patsubst %.c,%_c$(EXT),$(EXAMPLES_C_SOURCES))

EXAMPLES_CXX_SOURCES = $(wildcard $(EXAMPLES_PATH)/*.cpp)
EXAMPLES_CXX_TARGETS = $(patsubst %.cpp,%_cpp$(EXT),$(EXAMPLES_CXX_SOURCES))

EXAMPLES_TARGETS = $(EXAMPLES_C_TARGETS) $(EXAMPLES_CXX_TARGETS)

all: examples

examples: $(EXAMPLES_TARGETS)

$(EXAMPLES_PATH)/%_c$(EXT): $(EXAMPLES_PATH)/%.c $(SRC_PATH)/$(HEADER_NAME) $(SRC_PATH)/Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $<

$(EXAMPLES_PATH)/%_cpp$(EXT): $(EXAMPLES_PATH)/%.cpp $(SRC_PATH)/$(HEADER_NAME) $(SRC_PATH)/Makefile
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $<

clean:
ifeq ($(OS),Windows_NT)
	-del /Q ..\examples\*.exe 2>nul
else
	-rm -rf ../examples/*.bin > /dev/null 2>&1
endif