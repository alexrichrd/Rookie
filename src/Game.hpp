#pragma once
#include <memory>
#include <vector>

#include "Piece.hpp"

namespace chess {

class Player {
  Colour colour;
  std::string name;

 public:
  Player(Colour colour, std::string name)
      : colour(colour), name(std::move(name)) {}
  std::string get_name() { return this->name; }
};

enum GameStatus : std::int8_t { ONGOING, WHITE_WON, BLACK_WON, DRAW };

class Move {
  std::string white_move;
  std::string black_move;
  unsigned move_number;

 public:
  Move(std::string white_move, std::string black_move, unsigned move_number)
      : white_move(white_move),
        black_move(black_move),
        move_number(move_number) {};
};

class Game {
  unsigned move_count = 1;
  Colour next_to_move = WHITE;
  Player player_1;
  Player player_2;
  std::unique_ptr<Board> board_ptr;
  GameStatus status = ONGOING;
  std::vector<Move> moves;

  static std::array<std::array<Position, 8>, 8> makeBoard();

 public:
  Game(std::string name_1, std::string name_2);
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;
  Game(Game&&) noexcept = default;
  Game& operator=(Game&&) noexcept = default;
  GameStatus get_status();
  std::string get_player_name(const unsigned player_number);
  void add_move(Move&& move) {
    move_count++;
    moves.push_back(move);
  }
  unsigned get_move_count() { return move_count; }
  std::string to_string();
  Board* get_board_ptr() { return board_ptr.get(); }
  void set_next_to_move(Colour colour) { this->next_to_move = colour; }
};

}  // namespace chess