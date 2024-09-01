CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/proj.c src/builtins.c src/execute.c src/utils.c
OBJ = obj/proj.o obj/builtins.o obj/execute.o obj/utils.o
BIN = bin/proj

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj bin

run: all
	./bin/proj
