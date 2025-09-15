# Compiler
CC = gcc
CFLAGS = -Wall -O2 -Iinclude -DWINDOWS

# Source and Object files
SRC = src/main.c src/game.c src/hal.c
OBJ = $(SRC:.c=.o)
TARGET = retro_game.exe

.PHONY: all clean

# Build target
all: $(TARGET)
	@echo Build complete
	@del /Q src\*.o  # build 후 .o 파일 삭제

# Link
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean everything manually
clean:
	del /Q src\*.o $(TARGET)
	@echo Clean complete
