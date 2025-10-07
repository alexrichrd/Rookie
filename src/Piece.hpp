#pragma once
#include <print>

namespace chess {

class Board;
class Position;

enum Colour : std::int8_t { WHITE, BLACK };

class Piece {
 protected:
  Colour colour;
  std::string icon;

 public:
  Piece(Colour colour, std::string icon) : colour(colour), icon(icon) {};
  virtual ~Piece() = default;

  Colour get_Colour() { return colour; }
  virtual bool move(Board* board, Position* start_pos, Position* end_pos) = 0;
  std::string get_icon() { return this->icon; }
};

class Rook : public Piece {
 public:
  Rook(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2656" : "\u265C") {}
  bool move(Board* board, Position* start_pos, Position* end_pos) override;
};

class Knight : public Piece {
 public:
  Knight(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2658" : "\u265E") {}
  bool move(Board* /**/, Position* /**/, Position* /**/) { return false; }
};

class Bishop : public Piece {
 public:
  Bishop(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2657" : "\u265D") {}
  bool move(Board* /**/, Position* /**/, Position* /**/) { return false; }
};

class Queen : public Piece {
 public:
  Queen(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2655" : "\u265B") {}
  bool move(Board* /**/, Position* /**/, Position* /**/) { return false; }
};

class King : public Piece {
 public:
  King(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2654" : "\u265A") {}
  bool move(Board* /**/, Position* /**/, Position* /**/) { return false; }
};

class Pawn : public Piece {
 public:
  Pawn(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2659" : "\u265F") {}
  bool move(Board* /**/, Position* /**/, Position* /**/) { return false; }
};
}