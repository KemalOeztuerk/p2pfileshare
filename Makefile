# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -g $(shell pkg-config --cflags gtk+-3.0)
# Libraries
LDLIBS = -lm $(shell pkg-config --libs gtk+-3.0) -lpthread -lssl -lcrypto -lsqlite3 -lexpat -lminiupnpc

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
# Files shared between client and tracker
SHARED_SRCS = message.c metainfo.c peer.c tcp_client.c tcp_server.c utils.c
SHARED_OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SHARED_SRCS))

# Client-specific files
CLIENT_SRCS = gui.c talk_tracker.c download.c upload.c $(SHARED_SRCS)
CLIENT_OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(CLIENT_SRCS))

# Tracker-specific files
TRACKER_SRCS = track.c tracker.c $(SHARED_SRCS)
TRACKER_OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(TRACKER_SRCS))

GUI_SRCS = gui.c talk_tracker.c download.c upload.c $(SHARED_SRCS)
GUI_OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(GUI_SRCS))

# Executables
CLIENT_TARGET = $(BINDIR)/client
TRACKER_TARGET = $(BINDIR)/tracker
GUI_TARGET = $(BINDIR)/gui

# Ensure directories exist
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Default target
all: $(CLIENT_TARGET) $(TRACKER_TARGET)

# Linking rules for client
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Linking rules for tracker
$(TRACKER_TARGET): $(TRACKER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Linking rules for tracker
$(GUI_TARGET): $(GUIx_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Compilation rule for shared objects
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Compilation rule for client-specific objects
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Compilation rule for tracker-specific objects
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Compilation rule for tracker-specific objects
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
