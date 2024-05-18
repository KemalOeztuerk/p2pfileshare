CC = gcc
CFLAGS = -g -Wall

TARGET = program

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: $(BINDIR)/$(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/$(TARGET)

.PHONY: all clean
