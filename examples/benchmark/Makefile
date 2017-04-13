# Set up basic variables:
CC         = g++
CFLAGS     = -Wall -O2 -std=c++11

ifeq ($(DEBUG), 1)
    CFLAGS += -Wall -ggdb
endif

IFLAGS = -I../../include 

BIN = ./run_test
SRC = kdd_radius.cpp 

$(BIN): $(SRC) kdd_radius.h
	$(CC) $(CFLAGS) $(IFLAGS) $(SRC) -o $(BIN)
	chmod +x $(BIN)

phony: all clean clean.results

all: $(SRC)

clean: 
	rm $(BIN)

clean.results: 
	rm results/*.csv
