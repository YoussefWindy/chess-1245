# Compiler
CXX = g++-11
# Flags
CXXFLAGS = -std=c++20 -g -Wall -Werror=vla -MMD
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

# Directories
SRCDIR := src
INCDIR := include
OBJDIR := obj
BINDIR := bin

# Files
SOURCES := $(wildcard $(SRCDIR)/**/*.cc $(SRCDIR)/*.cc)
INCLUDE := $(wildcard $(INCDIR)/**/*.h $(INCDIR)/*.h $(INCDIR)/*.tpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.o, $(SOURCES))
DEPENDS := $(patsubst $(OBJDIR)/%.o, $(OBJDIR)/%.d, $(SOURCES))
EXEC := $(BINDIR)/chess

.PHONY: clean

all: $(EXEC)

$(EXEC): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)
