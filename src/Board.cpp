#include "Board.hpp"

#include "Piece.hpp"

using namespace chess;

std::string Position::get_icon() {
  if (piece_ptr) {
    return this->piece_ptr->get_icon();
  }
  return " ";
}

std::array<std::array<Position, 8>, 8> Board::makeBoard() {
  std::array<std::array<Position, 8>, 8> board{};
  std::array<char, 8> cols = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
  std::array<int, 8> rows = {1, 2, 3, 4, 5, 6, 7, 8};
  for (auto row : rows) {
    Colour current_colour = (row < 3) ? WHITE : BLACK;
    for (auto col : rows) {
      std::string icon = "";
      if (row == 1 || row == 8) {
        switch (col) {
          case 1:
          case 8:
            board[row - 1][col - 1] =
                Position(rows[row - 1], cols[col - 1],
                         std::make_unique<Rook>(current_colour));
            break;
          case 2:
          case 7:
            board[row - 1][col - 1] =
                Position(rows[row - 1], cols[col - 1],
                         std::make_unique<Knight>(current_colour));
            break;
          case 3:
          case 6:
            board[row - 1][col - 1] =
                Position(rows[row - 1], cols[col - 1],
                         std::make_unique<Bishop>(current_colour));
            break;
          case 4:
            icon = (current_colour == WHITE) ? "\u2655" : "\u265B";
            board[row - 1][col - 1] =
                Position(rows[row - 1], cols[col - 1],
                         std::make_unique<Queen>(current_colour));
            break;
          case 5:
            board[row - 1][col - 1] =
                Position(rows[row - 1], cols[col - 1],
                         std::make_unique<King>(current_colour));

            break;
          default:
            break;
        }
      } else if (row == 2 || row == 7) {
        icon = (current_colour == WHITE) ? "\u2659" : "\u265F";
        board[row - 1][col - 1] =
            Position(rows[row - 1], cols[col - 1],
                     std::make_unique<Pawn>(current_colour));

      } else {
        icon = " ";
        board[row - 1][col - 1] =
            Position(rows[row - 1], cols[col - 1], nullptr);
      }
    }
  }
  return board;
}

std::string Board::to_string() {
  std::string result = std::string("+------------------------+\n");
  for (size_t row = 0; row < 8; row++) {
    result += std::string("|");
    for (size_t col = 0; col < 8; col++) {
      result += "\u200A" + this->board[7 - row][col].get_icon() + "\u200A";
    }
    result += std::string("|\n");
  }
  result += std::string("+------------------------+\n");
  return result;
}
