# Compiler and flags
CC = gcc
CFLAGS = -Wall 
LDFLAGS = -lpthread

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Common source files
COMMON_SRCS = $(wildcard $(SRCDIR)/*.c)

# Client directories and files
CLIENT_TARGET = $(BINDIR)/client
CLIENT_SRCDIR = $(SRCDIR)/peer
CLIENT_OBJDIR = $(OBJDIR)/client
CLIENT_SRCS = $(wildcard $(CLIENT_SRCDIR)/*.c)
CLIENT_OBJS = $(patsubst $(SRCDIR)/%.c,$(CLIENT_OBJDIR)/%.o,$(COMMON_SRCS)) \
              $(patsubst $(CLIENT_SRCDIR)/%.c,$(CLIENT_OBJDIR)/%.o,$(CLIENT_SRCS))
CLIENT_LDFLAGS = -lpthread -lssl -lcrypto -lexpat

# Tracker directories and files
TRACKER_TARGET = $(BINDIR)/tracker
TRACKER_SRCDIR = $(SRCDIR)/tracker
TRACKER_OBJDIR = $(OBJDIR)/tracker
TRACKER_SRCS = $(wildcard $(TRACKER_SRCDIR)/*.c)
TRACKER_OBJS = $(patsubst $(SRCDIR)/%.c,$(TRACKER_OBJDIR)/%.o,$(COMMON_SRCS)) \
               $(patsubst $(TRACKER_SRCDIR)/%.c,$(TRACKER_OBJDIR)/%.o,$(TRACKER_SRCS))
TRACKER_LDFLAGS = -lpthread -lsqlite3 -lssl -lcrypto -lexpat

.PHONY: all clean

all: $(CLIENT_TARGET) $(TRACKER_TARGET)

# Ensure directories exist
$(OBJDIR) $(BINDIR) $(CLIENT_OBJDIR) $(TRACKER_OBJDIR):
	@mkdir -p $@

# Linking client executable
$(CLIENT_TARGET): $(CLIENT_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $@ $(CLIENT_LDFLAGS)

# Linking tracker executable
$(TRACKER_TARGET): $(TRACKER_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(TRACKER_OBJS) -o $@ $(TRACKER_LDFLAGS)

# Compiling common source files for client
$(CLIENT_OBJDIR)/%.o: $(SRCDIR)/%.c | $(CLIENT_OBJDIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compiling client source files
$(CLIENT_OBJDIR)/%.o: $(CLIENT_SRCDIR)/%.c | $(CLIENT_OBJDIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compiling common source files for tracker
$(TRACKER_OBJDIR)/%.o: $(SRCDIR)/%.c | $(TRACKER_OBJDIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compiling tracker source files
$(TRACKER_OBJDIR)/%.o: $(TRACKER_SRCDIR)/%.c | $(TRACKER_OBJDIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CLIENT_OBJDIR)/*.o $(TRACKER_OBJDIR)/*.o $(CLIENT_TARGET) $(TRACKER_TARGET)
	rm -rf $(OBJDIR) $(BINDIR)
