# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lpthread

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
FILES_DIR = files

# Target executable names
TARGET = $(BINDIR)/program

# Source files
SRC_FILES = $(wildcard $(SRCDIR)/*.c $(SRCDIR)/peer/*.c $(SRCDIR)/tracker/*.c)
OBJ_FILES = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC_FILES))

# Header files
HEADERS = $(wildcard $(SRCDIR)/*.h $(SRCDIR)/peer/*.h $(SRCDIR)/tracker/*.h)

# Default target
all: $(TARGET)

# Build target executable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build object files for peer
$(OBJDIR)/%.o: $(SRCDIR)/peer/%.c $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build object files for tracker
$(OBJDIR)/%.o: $(SRCDIR)/tracker/%.c $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Phony targets
.PHONY: all clean
