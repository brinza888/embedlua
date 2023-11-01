CC = gcc
CFLAGS = -Wall
LDFLAGS = -llua5.4

BIN ?= bin/

OBJECTS := main.o utils.o plugin.o command.o
OBJECTS := $(addprefix $(BIN), $(OBJECTS))

EXE := prog


.PHONY: all
all: $(EXE)


$(EXE): $(OBJECTS) $(BIN)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)


$(BIN)%.o: %.c $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@


$(BIN):
	mkdir -p $(BIN)


.PHONY: clean
clean:
	rm -rf $(BIN) $(EXE)

