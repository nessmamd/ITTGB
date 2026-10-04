CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic

TARGET = lexer
SOURCES = main.c Lexer.c AST.c
HEADERS = Lexer.h AST.h

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
