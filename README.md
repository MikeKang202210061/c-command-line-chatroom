# C Command-Line Chatroom

A terminal-based social messaging application written in C. It uses local files and in-memory structures to provide an offline chatroom-style experience without a network server.

## Features

- User registration and login
- Friend search, requests, and friend-list management
- Send, view, and delete messages
- Timestamped activity and local persistence
- Modular implementation split across authentication, social, messaging, and time utilities

## Build

```bash
make
./chatroom
```

Or compile directly:

```bash
gcc -std=c11 part1.c part2.c part3.c part4.c -o chatroom
```

Runtime account and message files are excluded from version control.
