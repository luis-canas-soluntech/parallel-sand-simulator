CC=gcc
CFLAGS= -Wall sdl2-config --cflags
LDFLAGS= sdl2-config --libs -lSDL2 -lSDL2_ttf 

SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c, src/%.o, $(SRC))
BIN=automata

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@ $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
clean:
	$(RM) -r $(BIN) $(OBJ)
