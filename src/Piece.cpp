#include "Piece.hpp"

#include <cstdlib>
#include <map>

#include "Board.hpp"

using namespace chess;

std::map<char, char> column_to_unsigned = {{'A', '1'}, {'B', '2'}, {'C', '3'},
                                           {'D', '4'}, {'E', '5'}, {'F', '6'},
                                           {'G', '7'}, {'H', '8'}};

bool chess::Rook::move(Board* board, Position* start_pos, Position* end_pos) {
  unsigned start_row = start_pos->get_row();
  unsigned start_col = column_to_unsigned[start_pos->get_column()];
  unsigned end_row = end_pos->get_row();
  unsigned end_col = column_to_unsigned[end_pos->get_column()];
  // check that there's a piece at start_pos
  if (!start_pos->get_raw_piece_ptr()) {
    return false;
  }
  // verify start_pos != end_pos
  if (start_row == end_row && start_col == end_col) {
    return false;
  }
  // rook has to move either along same column or same row, but not both
  bool moves_along_row = start_row == end_row;
  bool moves_along_column = start_col == end_col;
  if (!(moves_along_row ^ moves_along_column)) {
    return false;
  }
  // verify there are no other pieces along the way
  if (moves_along_row) {
    unsigned distance = (unsigned)std::abs((int)start_col - (int)end_col);
    if (distance > 1) {
      if (start_row < end_row) {
        for (unsigned i = 1; i < distance; ++i) {
          if (!(*board->get_board_ptr())[start_row + i][start_col]
                   .get_raw_piece_ptr()) {
            return false;
          }
        }
      } else {
        for (unsigned i = 1; i < distance; ++i) {
          if (!(*board->get_board_ptr())[start_row - i][start_col]
                   .get_raw_piece_ptr()) {
            return false;
          }
        }
      }
    }
  } else {
    unsigned distance = (unsigned)std::abs((int)start_col - (int)end_col);
    if (distance > 1) {
      if (start_col < end_col) {
        for (unsigned i = 1; i < distance; ++i) {
          if (!(*board->get_board_ptr())[start_row][start_col + i]
                   .get_raw_piece_ptr()) {
            return false;
          }
        }
      } else {
        for (unsigned i = 1; i < distance; ++i) {
          if (!(*board->get_board_ptr())[start_row][start_col - i]
                   .get_raw_piece_ptr()) {
            return false;
          }
        }
      }
    }
  }

  if (end_pos->get_raw_piece_ptr()) {
    // there is a piece at end_pos
    if (start_pos->get_raw_piece_ptr()->get_Colour() ==
        end_pos->get_raw_piece_ptr()->get_Colour()) {
      // check end_pos is not occupied with piece of own colour
      return false;
    }
  }

  end_pos->set_piece_ptr(start_pos->release_piece_ptr());
  return true;
}
