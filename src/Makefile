.PHONY: all examples clean

SRC_PATH 		?= ../src
EXAMPLES_PATH 	?= ../examples

CC = gcc
CFLAGS = -Wall -Wextra -std=c90 -g3 -pedantic
# CC = g++
# CFLAGS = -Wall -Wextra -std=c++11 -g3 -pedantic
INCLUDE = -I. -I$(SRC_PATH)
LINK = 

EXAMPLES_SOURCES = $(wildcard $(EXAMPLES_PATH)/*.c)
EXAMPLES_TARGETS = $(patsubst %.c,%.exe,$(EXAMPLES_SOURCES))

all: examples

examples: $(EXAMPLES_TARGETS)

$(EXAMPLES_PATH)/%.exe: $(EXAMPLES_PATH)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $<

clean:
	-del /Q $(subst /,\,$(EXAMPLES_PATH))\*.exe 2>nul