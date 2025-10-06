#include "Game.hpp"
#include <array>
#include <iostream>
#include <string>

using namespace chess;

std::array<std::array<chess::Piece, 8>, 8> Board::makeBoard() {
  std::array<std::array<chess::Piece, 8>, 8> board{};
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
          icon = (current_colour == WHITE) ? "\u2656" : "\u265C";
          board[row - 1][col - 1] = Piece(ROOK, current_colour, icon);
          break;
        case 2:
        case 7:
          icon = (current_colour == WHITE) ? "\u2658" : "\u265E";
          board[row - 1][col - 1] = Piece(KNIGHT, current_colour, icon);
          break;
        case 3:
        case 6:
          icon = (current_colour == WHITE) ? "\u2657" : "\u265D";
          board[row - 1][col - 1] = Piece(BISHOP, current_colour, icon);
          break;
        case 4:
          icon = (current_colour == WHITE) ? "\u2655" : "\u265B";
          board[row - 1][col - 1] = Piece(QUEEN, current_colour, icon);
          break;
        case 5:
          icon = (current_colour == WHITE) ? "\u2654" : "\u265A";
          board[row - 1][col - 1] = Piece(KING, current_colour, icon);
          break;
        default:
          break;
        }
      } else if (row == 2 || row == 7) {
        icon = (current_colour == WHITE) ? "\u2659" : "\u265F";
        board[row - 1][col - 1] = Piece(PAWN, current_colour, icon);
      } else {
        icon = " ";
        board[row - 1][col - 1] = Piece(NO_PIECE, NO_COL, icon);
      }
    }
  }
  return board;
}

std::string Board::to_string() {
  std::string result = "+------------------------+\n";
  for (size_t row = 0; row < 8; row++) {
    result += "|";
    for (size_t col = 0; col < 8; col++) {
      result += "\u200A" + this->board[7 - row][col].get_icon() + "\u200A";
    }
    result += "|\n";
  }
  result += "+------------------------+\n";
  return result;
}

bool chess::Move::valid_move(Colour next_to_move, Piece *start_piece,
                             Piece *end_piece) {
  if (start_piece->colour != next_to_move) {
    // wrong player makes a move
    return false;
  } else if (start_piece->colour == end_piece->colour) {
    // tries to capture one's own piece
    return false;
  } else if (start_piece->colour == NO_COL) {
    // there is no piece at the indicated position
    return false;
  }
}

int main() {
  // Get player names
  std::string name_1, name_2;
  std::println("Welcome! Player 1, please enter your name:");
  std::cin >> name_1;
  std::println("Player 2, please enter your name:");
  std::cin >> name_2;
  while (name_2 == name_1) {
    std::println("Player names must not be identical. Player 2, please enter a "
                 "new name:");
    std::cin >> name_2;
  }

  // Initialize game
  Game game(name_1, name_2);
  std::println("{}", game.get_board());
  while (game.get_status() == ONGOING) {
    /* make moves */
  };

  switch (game.get_status()) {
  case WHITE_WON:
    std::println("Player {} won, congratulations!", game.get_player_name(1));
    break;
  case BLACK_WON:
    std::println("Player {} won, congratulations!", game.get_player_name(2));
    break;
  case DRAW:
    std::println("Draw!");
    break;
  default:
    break;
  }

  return 0;
}