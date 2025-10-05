CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -pedantic -g
SRC = src/game.cpp
OUT = game

all: $(OUT)

$(OUT): $(SRC)
	clang-tidy $(SRC) -- $(CXXFLAGS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT)

format:
	clang-format -i $(SRC)

clean:
	rm -f $(OUT)
