CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRCDIR = src
PIECEDIR = $(SRCDIR)/pieces
TARGET = checkmate++

SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/board.cpp \
          $(SRCDIR)/game.cpp \
          $(SRCDIR)/player.cpp \
          $(PIECEDIR)/pawn.cpp \
          $(PIECEDIR)/rook.cpp \
          $(PIECEDIR)/knight.cpp \
          $(PIECEDIR)/bishop.cpp \
          $(PIECEDIR)/queen.cpp \
          $(PIECEDIR)/king.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

.PHONY: all clean run debug 