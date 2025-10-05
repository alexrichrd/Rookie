CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -pedantic -g
SRC = src/Game.cpp
HEADER = src/Game.hpp
OUT = Game

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT)

format:
	clang-format -i $(SRC) $(HEADER)

tidy:
	clang-tidy $(SRC) $(HEADER) -- $(CXXFLAGS)

clean:
	rm -f $(OUT)

.PHONY: all tidy format clean