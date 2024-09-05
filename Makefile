CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/proj.c src/builtins.c src/execute.c src/utils.c
OBJ = obj/proj.o obj/builtins.o obj/execute.o obj/utils.o
BIN = bin/proj

all: $(BIN)
	@echo "Build completed."

$(BIN): $(OBJ)
	@echo "Linking $(BIN)"
	@mkdir -p bin
	$(CC) -o $(BIN) $(OBJ)

obj/%.o: src/%.c
	@echo "Compiling $<"
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj bin

run: all
	./bin/proj

.PHONY: all clean run
