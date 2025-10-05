#include <print>

namespace chess {

enum PieceType : std::int8_t {
  PAWN,
  ROOK,
  KNIGHT,
  BISHOP,
  QUEEN,
  KING,
  NO_PIECE
};

enum Colour : std::int8_t { WHITE, BLACK, NO_COL };

class Piece {
public:
  PieceType piece_type;
  Colour colour;
  std::string icon;

public:
  Piece(PieceType piece_type, Colour colour, std::string icon)
      : piece_type(piece_type), colour(colour), icon(std::move(icon)) {}
};

class Position {
  char row;
  int col;
  Piece piece{NO_PIECE, NO_COL, " "};

public:
  Position() = default;
  Position(char row, int col, Piece piece)
      : row(row), col(col), piece(std::move(piece)) {}
  std::string get_icon() { return piece.icon; }
};

class Board {
  std::array<std::array<Position, 8>, 8> board;
  static std::array<std::array<Position, 8>, 8> makeBoard();

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
};

} // namespace chess