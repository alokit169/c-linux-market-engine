CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -Iinclude

TARGET = market_engine
SRC = src/main.c src/order.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)