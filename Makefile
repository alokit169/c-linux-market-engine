CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic

TARGET = market_engine
SRC = src/main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)