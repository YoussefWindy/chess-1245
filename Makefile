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
SOURCES := $(wildcard $(SRCDIR)/**/*.cc)
INCLUDE := $(wildcard $(INCDIR)/**/*.h $(INCDIR)/**/*.tpp)
OBJECTS := $(patsubst $(SRCDIR)/**/%.cc,$(OBJDIR)/%.o,$(SOURCES))
EXECUTABLE := $(BINDIR)/chess

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
