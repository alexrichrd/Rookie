#include "Piece.hpp"
#include <memory>

namespace chess {

class Board {
  std::array<std::array<Piece, 8>, 8> board;
  static std::array<std::array<Piece, 8>, 8> makeBoard();

public:
  Board() : board(makeBoard()) {};
  std::string to_string();
};

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
      : next_to_move(next_to_move), start_piece(start_piece),
        end_piece(end_piece),
        legal(valid_move(next_to_move, start_piece, end_piece)) {};
};

class Game {
  int move = 1;
  Colour next_to_move = WHITE;
  Board board{};
  Player player_1;
  Player player_2;
  GameStatus status = ONGOING;

public:
  Game(std::string name_1, std::string name_2)
      : player_1(WHITE, std::move(name_1)), player_2(BLACK, std::move(name_2)) {
  }

  std::string get_board() { return this->board.to_string(); }
  GameStatus get_status() { return this->status; }
  std::string get_player_name(const unsigned player_number) {
    if (player_number == 1) {
      return this->player_1.get_name();
    } else if (player_number == 2) {
      return this->player_2.get_name();
    }
  }
};

} // namespace chess