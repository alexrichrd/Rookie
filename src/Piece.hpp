#pragma once
#include <array>
#include <print>

#include "Position.hpp"

namespace chess {
using chess::BoardContext;
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
  virtual std::string move(chess::BoardContext& board_context,
                           Position& start_pos, Position& end_pos,
                           unsigned move_count) = 0;
  std::string get_icon() { return this->icon; }
};

class Rook : public Piece {
 public:
  Rook(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2656" : "\u265C") {}
  Colour get_Colour() override { return colour; }
  std::string move(chess::BoardContext& board_context, Position& start_pos,
                   Position& end_pos, unsigned move_count) override;
};

class Knight : public Piece {
 public:
  Knight(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2658" : "\u265E") {}
  Colour get_Colour() override { return colour; }
  std::string move(chess::BoardContext& board_context, Position& start_pos,
                   Position& end_pos, unsigned move_count);
};

class Bishop : public Piece {
 public:
  Bishop(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2657" : "\u265D") {}
  Colour get_Colour() override { return colour; }
  std::string move(chess::BoardContext& board_context, Position& start_pos,
                   Position& end_pos, unsigned move_count);
};

class Queen : public Piece {
 public:
  Queen(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2655" : "\u265B") {}
  Colour get_Colour() override { return colour; }
  std::string move(chess::BoardContext& board_context, Position& start_pos,
                   Position& end_pos, unsigned move_count);
};

class King : public Piece {
  bool in_check;

 public:
  King(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2654" : "\u265A") {}
  Colour get_Colour() override { return colour; }
  std::string move(chess::BoardContext& board_context, Position& start_pos,
                   Position& end_pos, unsigned move_count);
};

class Pawn : public Piece {
  unsigned en_passant_until_move = 0;

 public:
  Pawn(Colour colour)
      : Piece(colour, (colour == WHITE) ? "\u2659" : "\u265F") {}
  Colour get_Colour() override { return colour; }
  std::string move(chess::BoardContext& board_context, Position& start_pos,
                   Position& end_pos, unsigned move_count);
  void set_en_passant_susceptability(unsigned move_count) {
    en_passant_until_move = ++move_count;
  }
  unsigned get_en_passant_susceptability() { return en_passant_until_move; }
};
}  // namespace chess

namespace moveUtils {

using namespace chess;

bool valid_vertical_move(chess::BoardContext& board_context, unsigned start_row,
                         unsigned start_col, unsigned end_row,
                         unsigned end_col);
bool valid_horizontal_move(chess::BoardContext& board_context,
                           unsigned start_row, unsigned start_col,
                           unsigned end_row, unsigned end_col);
bool valid_diagonal_move(chess::BoardContext& board_context, unsigned start_row,
                         unsigned start_col, unsigned end_row,
                         unsigned end_col);
}  // namespace moveUtils