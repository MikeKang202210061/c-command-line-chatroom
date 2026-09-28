CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
TARGET = chatroom
SOURCES = part1.c part2.c part3.c part4.c

$(TARGET): $(SOURCES) project.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o
