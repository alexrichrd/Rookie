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
  PieceType piece_type = NO_PIECE;
  Colour colour = NO_COL;
  std::string icon = "";

public:
  Piece() = default;
  Piece(PieceType piece_type, Colour colour, std::string icon)
      : piece_type(piece_type), colour(colour), icon(std::move(icon)) {};
  std::string get_icon() { return icon; }

  friend class Move;
};

} // namespace chess