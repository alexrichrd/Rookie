#include "Piece.hpp"

#include <cstdlib>
#include <map>

using namespace chess;
using namespace moveUtils;

bool moveUtils::valid_vertical_move(chess::BoardContext& board_context,
                                    unsigned start_row, unsigned start_col,
                                    unsigned end_row, unsigned end_col) {
  bool moves_along_column = start_col == end_col;
  if (!moves_along_column) {
    return false;
  }
  unsigned distance = (unsigned)std::abs((int)start_col - (int)end_col);
  if (distance > 1) {
    if (start_row < end_row) {
      for (unsigned i = 1; i < distance; ++i) {
        if (board_context.board[start_row + i][start_col].get_piece_ptr()) {
          return false;
        }
      }
    } else {
      for (unsigned i = 1; i < distance; ++i) {
        if (board_context.board[start_row - i][start_col].get_piece_ptr()) {
          return false;
        }
      }
    }
  }

  return true;
}

bool moveUtils::valid_horizontal_move(chess::BoardContext& board_context,
                                      unsigned start_row, unsigned start_col,
                                      unsigned end_row, unsigned end_col) {
  bool moves_along_row = start_row == end_row;
  if (!moves_along_row) {
    return false;
  }
  unsigned distance = (unsigned)std::abs((int)start_col - (int)end_col);
  if (distance > 1) {
    if (start_row < end_row) {
      for (unsigned i = 1; i < distance; ++i) {
        if (board_context.board[start_row][start_col + i].get_piece_ptr()) {
          return false;
        }
      }
    } else {
      for (unsigned i = 1; i < distance; ++i) {
        if (board_context.board[start_row][start_col - i].get_piece_ptr()) {
          return false;
        }
      }
    }
  }
  return true;
}

bool moveUtils::valid_diagonal_move(chess::BoardContext& board_context,
                                    unsigned start_row, unsigned start_col,
                                    unsigned end_row, unsigned end_col) {
  int col_diff = std::abs((int)start_col - (int)end_col);
  int row_diff = std::abs((int)start_row - (int)end_row);
  if (!(col_diff == row_diff)) {
    return false;
  }
  int sign_vertical = (end_row - start_row > 0) ? 1 : -1;
  int sign_horizontal = (end_col - start_col > 0) ? 1 : -1;
  // verify there are no other pieces along the way
  for (unsigned i = 1; i < (unsigned)col_diff; ++i) {
    if (board_context
            .board[start_row + i * sign_vertical]
                  [start_col + i * sign_horizontal]
            .get_piece_ptr()) {
      return false;
    }
  }
  return true;
}

std::string chess::Rook::move(BoardContext& board_context, Position& start_pos,
                              Position& end_pos, unsigned /**/) {
  unsigned start_row = start_pos.get_row();
  unsigned start_col = start_pos.get_column();
  unsigned end_row = end_pos.get_row();
  unsigned end_col = end_pos.get_column();
  // rook has to move either along same column or same row, but not both
  bool moves_along_row = start_row == end_row;
  bool moves_along_column = start_col == end_col;
  bool valid_horizontal_move = false;
  bool valid_vertical_move = false;
  if (moves_along_row) {
    valid_horizontal_move = moveUtils::valid_horizontal_move(
        board_context, start_row, start_col, end_row, end_col);
  } else if (moves_along_column) {
    valid_vertical_move = moveUtils::valid_vertical_move(
        board_context, start_row, start_col, end_row, end_col);
  } else {
    return "Illegal move: Rook is neither moved horizontally nor vertically.";
  }

  if (valid_horizontal_move ^ valid_vertical_move) {
    end_pos.set_piece_ptr(start_pos.release_piece_ptr());
    return "Legal move";
  } else {
    return "Illegal move: horizontal or vertical rook move could not be "
           "performed.";
  }
}

std::string chess::Bishop::move(BoardContext& board_context,
                                Position& start_pos, Position& end_pos,
                                unsigned /**/) {
  unsigned start_row = start_pos.get_row();
  unsigned start_col = start_pos.get_column();
  unsigned end_row = end_pos.get_row();
  unsigned end_col = end_pos.get_column();
  // the absolute difference in both vertical and horizontal direction must be
  // the same
  int valid_diagonal_move = moveUtils::valid_diagonal_move(
      board_context, start_row, start_col, end_row, end_col);
  if (!valid_diagonal_move) {
    return "Illegal move: tried to move bishop non-diagonally";
  }
  end_pos.set_piece_ptr(start_pos.release_piece_ptr());
  return "Legal move";
}

std::string chess::Queen::move(BoardContext& board_context, Position& start_pos,
                               Position& end_pos, unsigned /**/) {
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
          board_context, start_row, start_col, end_row, end_col);
    } else {
      valid_vertical_move = moveUtils::valid_vertical_move(
          board_context, start_row, start_col, end_row, end_col);
    }
    if (!(valid_horizontal_move ^ valid_vertical_move)) {
      return "Illegal move: tried to move Queen neither like a rook nor like a "
             "bishop";
    }
    end_pos.set_piece_ptr(start_pos.release_piece_ptr());
    return "Legal move";
  }
  // determine if Queen moves like a bishop
  int valid_diagonal_move = moveUtils::valid_diagonal_move(
      board_context, start_row, start_col, end_row, end_col);
  if (!valid_diagonal_move) {
    return "Illegal move: tried to move Queen neither like a rook nor like a "
           "bishop";
  }
  end_pos.set_piece_ptr(start_pos.release_piece_ptr());
  return "Legal move";
}

std::string chess::Knight::move(BoardContext& /**/, Position& start_pos,
                                Position& end_pos, unsigned /**/) {
  unsigned start_row = start_pos.get_row();
  unsigned start_col = start_pos.get_column();
  unsigned end_row = end_pos.get_row();
  unsigned end_col = end_pos.get_column();
  // knight has to move in an l-shape:
  // either end_col differs from start_col by two and
  // end_row differs from start_row by one (or vice versa)
  int col_diff = std::abs((int)start_col - (int)end_col);
  int row_diff = std::abs((int)start_row - (int)end_row);
  if (!((col_diff == 2 && row_diff == 1) || (col_diff == 1 && row_diff == 2))) {
    return "Illegal knight move";
  }

  end_pos.set_piece_ptr(start_pos.release_piece_ptr());
  return "Legal move";
}

std::string chess::King::move(BoardContext& board_context, Position& start_pos,
                              Position& end_pos, unsigned /**/) {
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
    return "Illegal king move";
  }
  // perform move and update king position pointer
  if (start_pos.get_piece_ptr()->get_Colour() == WHITE) {
    board_context.ptr_to_white_king_position = &end_pos;
  } else {
    board_context.ptr_to_black_king_position = &end_pos;
  }
  end_pos.set_piece_ptr(start_pos.release_piece_ptr());
  return "Legal move";
}

std::string chess::Pawn::move(BoardContext& board_context, Position& start_pos,
                              Position& end_pos, unsigned move_count) {
  unsigned start_row = start_pos.get_row();
  unsigned start_column = start_pos.get_column();
  unsigned end_row = end_pos.get_row();
  unsigned end_column = end_pos.get_column();
  Colour start_colour = this->colour;
  Piece* end_pos_piece_ptr = end_pos.get_piece_ptr();
  unsigned distance_vertical =
      (unsigned)std::abs((int)start_row - (int)end_row);
  unsigned distance_horizontal =
      (unsigned)std::abs((int)start_column - (int)end_column);
  if (start_colour == WHITE) {
    // check if pawn moves in right direction
    if (start_row >= end_row || distance_horizontal > 1) {
      return "Illegal pawn move";
    }
    // pawn moves one step forward
    if (distance_vertical == 1 && distance_horizontal == 0) {
      // check if target is empty
      if (end_pos_piece_ptr) {
        return "Illegal pawn move";
      }
      end_pos.set_piece_ptr(start_pos.release_piece_ptr());
      return "Legal move";
      // pawn moves two steps forward
    } else if (distance_vertical == 2 && distance_horizontal == 0) {
      // check if start_pos was in row 1
      if (start_row != 1) {
        return "Illegal pawn move";
      }
      // check if target is empty
      if (end_pos_piece_ptr) {
        return "Illegal pawn move";
      }
      this->set_en_passant_susceptability(move_count);
      end_pos.set_piece_ptr(start_pos.release_piece_ptr());
      return "Legal move";
    } else if (distance_vertical == 1 && distance_horizontal == 1) {
      if (!end_pos_piece_ptr) {
        // check for en passant
        Pawn* en_passant_target = dynamic_cast<Pawn*>(
            board_context.board[start_row][end_column].get_piece_ptr());
        if (en_passant_target) {
          if (move_count ==
              en_passant_target->get_en_passant_susceptability()) {
            board_context.board[start_row][end_column].release_piece_ptr();
            end_pos.set_piece_ptr(start_pos.release_piece_ptr());
            return "Legal move";
          }
        }
        // otherwise move is illegal
        return "Illegal move: diagonal pawn move without capture";
      } else if (end_pos_piece_ptr->get_Colour() == BLACK) {
        // check if target contains a piece of a different colour
        end_pos.set_piece_ptr(start_pos.release_piece_ptr());
        return "Legal move";
      } else {
        return "Illegal pawn move";
      }
    }
    return "Illegal pawn move";
    // same for black
  } else {
    // check if pawn moves in right direction
    if (start_row <= end_row || distance_horizontal > 1) {
      return "Illegal pawn move";
    }
    // pawn moves one step forward
    if (distance_vertical == 1 && distance_horizontal == 0) {
      // check if target is empty
      if (end_pos_piece_ptr) {
        return "Illegal pawn move";
      }
      end_pos.set_piece_ptr(start_pos.release_piece_ptr());
      return "Legal move";
      // pawn moves two steps forward
    } else if (distance_vertical == 2 && distance_horizontal == 0) {
      // check if start_pos was in row 2
      if (start_row != 6) {
        return "Illegal pawn move";
      }
      // check if target is empty
      if (end_pos_piece_ptr) {
        return "Illegal pawn move";
      }
      this->set_en_passant_susceptability(move_count);
      end_pos.set_piece_ptr(start_pos.release_piece_ptr());
      return "Legal move";
    } else if (distance_vertical == 1 && distance_horizontal == 1) {
      if (!end_pos_piece_ptr) {
        // check for en passant
        Pawn* en_passant_target = dynamic_cast<Pawn*>(
            board_context.board[start_row][end_column].get_piece_ptr());
        if (en_passant_target) {
          if (move_count ==
              en_passant_target->get_en_passant_susceptability()) {
            board_context.board[start_row][end_column].release_piece_ptr();
            end_pos.set_piece_ptr(start_pos.release_piece_ptr());
            return "Legal move";
          }
        }
        // otherwise move is illegal
        return "Illegal move: diagonal pawn move without capture";
      } else if (end_pos_piece_ptr->get_Colour() == WHITE) {
        // check if target contains a piece of a different colour
        end_pos.set_piece_ptr(start_pos.release_piece_ptr());
        return "Legal move";
      } else {
        return "Illegal pawn move";
      }
    }
    return "Illegal pawn move";
  }
  return "Illegal pawn move";
}
