# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11

# Directories
SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Output binary name
TARGET = Jack

# Debug build settings
DEBUG_CXXFLAGS = $(CXXFLAGS) -g -DDEBUG
DEBUG_TARGET = $(BINDIR)/$(TARGET:.o=_debug.o)

# Release build settings
RELEASE_CXXFLAGS = $(CXXFLAGS) -O2
RELEASE_TARGET = $(BINDIR)/$(TARGET:.o=_release.o)

SOURCES = \
    $(SRCDIR)/Parser/Parser.cpp \
    $(SRCDIR)/Lexer/Lexer.cpp \
    $(SRCDIR)/Types/ParseTree.cpp \
    $(SRCDIR)/Types/Token.cpp \
    $(SRCDIR)/main.cpp \
    $(SRCDIR)/CompilerOut/PrintTypes.cpp

.PHONY: all clean

all: debug release

debug: $(DEBUG_TARGET)

release: $(RELEASE_TARGET)

$(DEBUG_TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(DEBUG_CXXFLAGS) $^ -o $@

$(RELEASE_TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(RELEASE_CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)