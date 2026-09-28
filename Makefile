CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
TARGET = chatroom
SOURCES = authentication.c contacts.c messages.c time_utils.c

$(TARGET): $(SOURCES) chat_system.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o
