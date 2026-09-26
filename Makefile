CC = gcc
CFLAGS = -std=gnu17 -Wall -g

TARGET = main
SRCS = main.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
