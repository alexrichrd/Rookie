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
  std::unique_ptr<BoardContext> board_context_ptr;
  GameStatus status = ONGOING;
  std::vector<Move> moves;

  static BoardContext make_board_context();

 public:
  Game(std::string name_1, std::string name_2)
      : player_1(WHITE, std::move(name_1)),
        player_2(BLACK, std::move(name_2)),
        board_context_ptr(
            std::make_unique<BoardContext>(make_board_context())) {};
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;
  Game(Game&&) noexcept = default;
  Game& operator=(Game&&) noexcept = default;
  GameStatus get_status() { return this->status; }
  std::string get_player_name(const unsigned player_number) {
    if (player_number == 1) {
      return this->player_1.get_name();
    }
    return this->player_2.get_name();
  }
  void add_move(Move&& move) {
    move_count++;
    moves.push_back(move);
  }
  unsigned get_move_count() { return move_count; }
  std::string to_string();
  BoardContext* get_board_context_ptr() { return board_context_ptr.get(); }
  void set_next_to_move(Colour colour) { this->next_to_move = colour; }
  Colour get_next_to_move() { return this->next_to_move; }
  static void play_game();
};

}  // namespace chess