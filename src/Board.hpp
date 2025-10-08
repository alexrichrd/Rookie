#pragma once
#include <memory>

namespace chess {

class Piece;

class Position {
  unsigned row;
  char column;
  std::unique_ptr<Piece> piece_ptr;

 public:
  Position() = default;
  Position(unsigned row, char column, std::unique_ptr<Piece> piece_ptr)
      : row(row), column(column), piece_ptr(std::move(piece_ptr)) {};
  Position(const Position&) = delete;
  Position& operator=(const Position& other) = delete;
  Position& operator=(Position&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    row = other.row;
    column = other.column;
    piece_ptr = std::move(other.piece_ptr);
    return *this;
  }
  Position(Position&& other) noexcept
      : row(other.row),
        column(other.column),
        piece_ptr(std::move(other.piece_ptr)) {}

  std::string get_icon();
  unsigned get_row() { return row; }
  char get_column() { return column; }
  Piece* get_raw_piece_ptr() { return piece_ptr.get(); }
  std::unique_ptr<Piece> release_piece_ptr() { return std::move(piece_ptr); }
  void set_piece_ptr(std::unique_ptr<Piece> piece_ptr_) {
    piece_ptr = std::move(piece_ptr_);
  }
};

class Board {
  static std::array<std::array<Position, 8>, 8> makeBoard();

 public:
  std::array<std::array<Position, 8>, 8> board;
  Board() : board(makeBoard()) {};
  std::string to_string();
  std::array<std::array<Position, 8>, 8>* get_board_ptr() { return &board; }
};
}  // namespace chess