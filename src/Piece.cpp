#include "Piece.hpp"

#include <cstdlib>
#include <map>

bool moveUtils::valid_vertical_move(chess::Board& board, unsigned start_row,
                                    unsigned start_col, unsigned end_row,
                                    unsigned end_col) {
  bool moves_along_column = start_col == end_col;
  if (!moves_along_column) {
    return false;
  }
  unsigned distance = (unsigned)std::abs((int)start_col - (int)end_col);
  if (distance > 1) {
    if (start_row < end_row) {
      for (unsigned i = 1; i < distance; ++i) {
        if (board.at(start_row + i).at(start_col).get_piece_ptr()) {
          return false;
        }
      }
    } else {
      for (unsigned i = 1; i < distance; ++i) {
        if (board.at(start_row - i).at(start_col).get_piece_ptr()) {
          return false;
        }
      }
    }
  }

  return true;
}

bool moveUtils::valid_horizontal_move(chess::Board& board, unsigned start_row,
                                      unsigned start_col, unsigned end_row,
                                      unsigned end_col) {
  bool moves_along_row = start_row == end_row;
  if (!moves_along_row) {
    return false;
  }
  unsigned distance = (unsigned)std::abs((int)start_col - (int)end_col);
  if (distance > 1) {
    if (start_row < end_row) {
      for (unsigned i = 1; i < distance; ++i) {
        if (board.at(start_row).at(start_col + i).get_piece_ptr()) {
          return false;
        }
      }
    } else {
      for (unsigned i = 1; i < distance; ++i) {
        if (board.at(start_row).at(start_col - i).get_piece_ptr()) {
          return false;
        }
      }
    }
  }
  return true;
}

bool moveUtils::valid_diagonal_move(chess::Board& board, unsigned start_row,
                                    unsigned start_col, unsigned end_row,
                                    unsigned end_col) {
  int col_diff = std::abs((int)start_col - (int)end_col);
  int row_diff = std::abs((int)start_row - (int)end_row);
  if (!(col_diff == row_diff)) {
    return false;
  }
  int sign_vertical = (end_row - start_row > 0) ? 1 : -1;
  int sign_horizontal = (end_col - start_col > 0) ? 1 : -1;
  // verify there are no other pieces along the way
  for (unsigned i = 1; i < (unsigned)col_diff; ++i) {
    if (board.at(start_row + i * sign_vertical)
            .at(start_col + i * sign_horizontal)
            .get_piece_ptr()) {
      return false;
    }
  }
  return true;
}

using namespace chess;

bool chess::Rook::move(Board& board, Position& start_pos, Position& end_pos) {
  unsigned start_row = start_pos.get_row();
  unsigned start_col = start_pos.get_column();
  unsigned end_row = end_pos.get_row();
  unsigned end_col = end_pos.get_column();
  // rook has to move either along same column or same row, but not both
  bool moves_along_column = start_row == end_row;
  bool moves_along_row = start_col == end_col;
  bool valid_horizontal_move = false;
  bool valid_vertical_move = false;
  if (moves_along_row) {
    valid_horizontal_move = moveUtils::valid_horizontal_move(
        board, start_row, start_col, end_row, end_col);
  } else if (moves_along_column) {
    valid_vertical_move = moveUtils::valid_vertical_move(
        board, start_row, start_col, end_row, end_col);
  }
  if (!(valid_horizontal_move ^ valid_vertical_move)) {
    return false;
  }
  end_pos.set_piece_ptr(start_pos.release_piece_ptr());
  return true;
}

bool chess::Bishop::move(Board& board, Position& start_pos, Position& end_pos) {
  unsigned start_row = start_pos.get_row();
  unsigned start_col = start_pos.get_column();
  unsigned end_row = end_pos.get_row();
  unsigned end_col = end_pos.get_column();
  // the absolute difference in both vertical and horizontal direction must be
  // the same
  int valid_diagonal_move = moveUtils::valid_diagonal_move(
      board, start_row, start_col, end_row, end_col);
  if (!valid_diagonal_move) {
    return false;
  }
  end_pos.set_piece_ptr(start_pos.release_piece_ptr());
  return true;
}

bool chess::Queen::move(Board& board, Position& start_pos, Position& end_pos) {
  unsigned start_row = start_pos.get_row();
  unsigned start_col = start_pos.get_column();
  unsigned end_row = end_pos.get_row();
  unsigned end_col = end_pos.get_column();
  // determine if Queen moves like a rook
  bool moves_along_column = start_row == end_row;
  bool moves_along_row = start_col == end_col;
  if (moves_along_column || moves_along_row) {
    bool valid_horizontal_move = false;
    bool valid_vertical_move = false;
    if (moves_along_row) {
      valid_horizontal_move = moveUtils::valid_horizontal_move(
          board, start_row, start_col, end_row, end_col);
    } else {
      valid_vertical_move = moveUtils::valid_vertical_move(
          board, start_row, start_col, end_row, end_col);
    }
    if (!(valid_horizontal_move ^ valid_vertical_move)) {
      return false;
    }
    end_pos.set_piece_ptr(start_pos.release_piece_ptr());
    return true;
  }
  // determine if Queen moves like a bishop
  int valid_diagonal_move = moveUtils::valid_diagonal_move(
      board, start_row, start_col, end_row, end_col);
  if (!valid_diagonal_move) {
    return false;
  }
  end_pos.set_piece_ptr(start_pos.release_piece_ptr());
  return true;
}

bool chess::Knight::move(Board& /**/, Position& start_pos, Position& end_pos) {
  unsigned start_row = start_pos.get_row();
  unsigned start_col = start_pos.get_column();
  unsigned end_row = end_pos.get_row();
  unsigned end_col = end_pos.get_column();
  // knight has to move in an l-shape:
  // either end_col differs from start_col by two and
  // end_row differs from start_row by one (or vice versa)
  int col_diff = std::abs((int)start_col - (int)end_col);
  int row_diff = std::abs((int)start_row - (int)end_row);
  if (!(col_diff == 2 && row_diff == 1) || (col_diff == 1 && row_diff == 2)) {
    return false;
  }

  end_pos.set_piece_ptr(start_pos.release_piece_ptr());
  return true;
}

bool chess::King::move(Board& /**/, Position& start_pos, Position& end_pos) {
  unsigned start_row = start_pos.get_row();
  unsigned start_col = start_pos.get_column();
  unsigned end_row = end_pos.get_row();
  unsigned end_col = end_pos.get_column();
  // check whether King only moves one step
  unsigned distance_vertical =
      (unsigned)std::abs((int)start_row - (int)end_row);
  unsigned distance_horizontal =
      (unsigned)std::abs((int)start_col - (int)end_col);
  if (!(distance_horizontal <= 1 && distance_vertical <= 1)) {
    return false;
  }
  end_pos.set_piece_ptr(start_pos.release_piece_ptr());
  return true;
}

bool chess::Pawn::move(Board& /**/, Position& start_pos, Position& end_pos) {
  unsigned start_row = start_pos.get_row();
  unsigned start_col = start_pos.get_column();
  unsigned end_row = end_pos.get_row();
  unsigned end_col = end_pos.get_column();
  unsigned distance_vertical =
      (unsigned)std::abs((int)start_row - (int)end_row);
  unsigned distance_horizontal =
      (unsigned)std::abs((int)start_col - (int)end_col);
  if (this->colour == WHITE) {
    // check if pawn moves in right direction
    if (start_row >= end_row || distance_horizontal > 1) {
      return false;
    }
    // pawn moves one step forward
    if (distance_vertical == 1 && distance_horizontal == 0) {
      // check if target is empty
      if (end_pos.get_piece_ptr()) {
        return false;
      }
      end_pos.set_piece_ptr(start_pos.release_piece_ptr());
      return true;
      // pawn moves two steps forward
    } else if (distance_vertical == 1 && distance_horizontal == 0) {
      // check if start_pos was in row 2
      if (start_pos.get_row() != 2) {
        return false;
      }
      // check if target is empty
      if (end_pos.get_piece_ptr()) {
        return false;
      }
      end_pos.set_piece_ptr(start_pos.release_piece_ptr());
      return true;
    } else if (distance_vertical == 1 && distance_horizontal == 1) {
      // check if target contains a piece of a different colour
      if (end_pos.get_piece_ptr()->get_Colour() == BLACK) {
        end_pos.set_piece_ptr(start_pos.release_piece_ptr());
        return true;
      }
    }
    return false;
    // same for black
  } else {
    // check if pawn moves in right direction
    if (start_row <= end_row || distance_horizontal > 1) {
      return false;
    }
    // pawn moves one step forward
    if (distance_vertical == 1 && distance_horizontal == 0) {
      // check if target is empty
      if (end_pos.get_piece_ptr()) {
        return false;
      }
      end_pos.set_piece_ptr(start_pos.release_piece_ptr());
      return true;
      // pawn moves two steps forward
    } else if (distance_vertical == 1 && distance_horizontal == 0) {
      // check if start_pos was in row 2
      if (start_pos.get_row() != 7) {
        return false;
      }
      // check if target is empty
      if (end_pos.get_piece_ptr()) {
        return false;
      }
      end_pos.set_piece_ptr(start_pos.release_piece_ptr());
      return true;
    } else if (distance_vertical == 1 && distance_horizontal == 1) {
      // check if target contains a piece of a different colour
      if (end_pos.get_piece_ptr()->get_Colour() == BLACK) {
        end_pos.set_piece_ptr(start_pos.release_piece_ptr());
        return true;
      }
    }
    return false;
  }
  return false;
}
