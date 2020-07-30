# By James Henderson, 2020

CXX := gcc
LXX = gcc

CXXFLAGS := -Os 
LXXFLAGS := -s -Os


BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj

SRCS := $(notdir $(shell find -name '*.c'))
OBJS := $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

zettelkasten: builddir $(OBJS) $(SRCS) 
	$(LXX) $(LXXFLAGS) $(OBJS)  -o $(BUILDDIR)/zk -lmenu -lncurses

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ -lmenu -lncurses

.PHONY: builddir
builddir:
	@mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	@rm -rf build/