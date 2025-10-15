#pragma once
#include <memory>

namespace chess {

class Piece;

class Position {
  // chess positions are internally represented as array positions,
  // e.g. position c4 is 23 (column, row)
  unsigned row;
  unsigned column;
  std::unique_ptr<Piece> piece_ptr;

 public:
  Position() = default;
  Position(unsigned row, unsigned column, std::unique_ptr<Piece> piece_ptr)
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

  unsigned get_row() { return row; }
  unsigned get_column() { return column; }
  Piece* get_piece_ptr() { return piece_ptr.get(); }
  std::unique_ptr<Piece> release_piece_ptr() { return std::move(piece_ptr); }
  void set_piece_ptr(std::unique_ptr<Piece> piece_ptr_) {
    piece_ptr = std::move(piece_ptr_);
  }
};

struct BoardContext {
  std::array<std::array<Position, 8>, 8> board;
  Position* ptr_to_white_king_position = nullptr;
  Position* ptr_to_black_king_position = nullptr;
};

}  // namespace chess