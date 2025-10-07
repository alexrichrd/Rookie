#pragma once
#include <memory>

#include "Piece.hpp"

namespace chess {

class Board;

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
  Colour next_to_move;
  Piece *start_piece;
  Piece *end_piece;
  bool legal;

  static bool valid_move(Colour next_to_move, Piece *start_piece,
                         Piece *end_piece);

 public:
  Move(Colour next_to_move, Piece *start_piece, Piece *end_piece)
      : next_to_move(next_to_move),
        start_piece(start_piece),
        end_piece(end_piece),
        legal(valid_move(next_to_move, start_piece, end_piece)) {};
};

class Game {
  unsigned move = 1;
  Colour next_to_move = WHITE;
  Player player_1;
  Player player_2;
  std::unique_ptr<Board> board_ptr;
  GameStatus status = ONGOING;

 public:
  Game(std::string name_1, std::string name_2);
  std::string get_board();
  GameStatus get_status();
  std::string get_player_name(const unsigned player_number);
};

}  // namespace chess