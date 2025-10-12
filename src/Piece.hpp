#pragma once
#include <array>
#include <print>

#include "Position.hpp"

namespace chess {

using Board = std::array<std::array<Position, 8>, 8>;
enum Colour : std::int8_t { WHITE, BLACK };

constexpr std::string_view ColourToString(Colour colour) noexcept {
  switch (colour) {
    case WHITE:
      return "WHITE";
      break;

    case BLACK:
      return "BLACK";
      break;

    default:
      return "error: invalid colour";
  }
}

class Piece {
 protected:
  Colour colour;
  std::string icon;

 public:
  Piece(Colour colour, std::string icon) : colour(colour), icon(icon) {};
  virtual ~Piece() = default;

  virtual Colour get_Colour() = 0;
  virtual std::string move(Board& board, Position& start_pos,
                           Position& end_pos) = 0;
  std::string get_icon() { return this->icon; }
};

class Rook : public Piece {
 public:
  Rook(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2656" : "\u265C") {}
  Colour get_Colour() override { return colour; }
  std::string move(Board& board, Position& start_pos,
                   Position& end_pos) override;
};

class Knight : public Piece {
 public:
  Knight(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2658" : "\u265E") {}
  Colour get_Colour() override { return colour; }
  std::string move(Board& board, Position& start_pos, Position& end_pos);
};

class Bishop : public Piece {
 public:
  Bishop(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2657" : "\u265D") {}
  Colour get_Colour() override { return colour; }
  std::string move(Board& board, Position& start_pos, Position& end_pos);
};

class Queen : public Piece {
 public:
  Queen(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2655" : "\u265B") {}
  Colour get_Colour() override { return colour; }
  std::string move(Board& board, Position& start_pos, Position& end_pos);
};

class King : public Piece {
 public:
  King(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2654" : "\u265A") {}
  Colour get_Colour() override { return colour; }
  std::string move(Board& board, Position& start_pos, Position& end_pos);
};

class Pawn : public Piece {
 public:
  Pawn(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2659" : "\u265F") {}
  Colour get_Colour() override { return colour; }
  std::string move(Board& board, Position& start_pos, Position& end_pos);
};
}  // namespace chess

namespace moveUtils {
bool valid_vertical_move(chess::Board& board, unsigned start_row,
                         unsigned start_col, unsigned end_row,
                         unsigned end_col);
bool valid_horizontal_move(chess::Board& board, unsigned start_row,
                           unsigned start_col, unsigned end_row,
                           unsigned end_col);
bool valid_diagonal_move(chess::Board& board, unsigned start_row,
                         unsigned start_col, unsigned end_row,
                         unsigned end_col);
}  // namespace moveUtils