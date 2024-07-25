# Compiler
CXX = g++-11
# Flags
CXXFLAGS = -std=c++20 -g -Wall -Werror=vla -MMD
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

# Directories
SRCDIR := src
INCDIR := include
OBJDIR := obj

# Files
SOURCES := $(wildcard $(SRCDIR)/**/*.cc $(SRCDIR)/*.cc)
INCLUDE := $(wildcard $(INCDIR)/**/*.h $(INCDIR)/*.h $(INCDIR)/*.tpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.o, $(SOURCES))
DEPENDS := $(patsubst $(OBJDIR)/%.o, $(OBJDIR)/%.d, $(SOURCES))
EXEC := chess

.PHONY: clean

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) -lX11

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(EXEC)
