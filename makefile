# By James Henderson, 2020

CC := gcc

CFLAGS := -std=c99 -Os 
LFLAGS := -s -Os


BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj

SRCS := $(notdir $(shell find -name '*.c'))
OBJS := $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

zettelkasten: builddir $(OBJS) $(SRCS) 
	$(CC) $(LFLAGS) $(OBJS)  -o $(BUILDDIR)/zk -lmenu -lncurses

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $^ -o $@ -lmenu -lncurses

.PHONY: builddir
builddir:
	@mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	@rm -rf build/
