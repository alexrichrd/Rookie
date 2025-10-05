#include "Game.hpp"
#include <array>
#include <iostream>
#include <string>

using namespace chess;

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
          icon = (current_colour == WHITE) ? "\u2656" : "\u265C";
          board[row - 1][col - 1] =
              Position(row, cols[col - 1], Piece(ROOK, current_colour, icon));
          break;
        case 2:
        case 7:
          icon = (current_colour == WHITE) ? "\u2658" : "\u265E";
          board[row - 1][col - 1] =
              Position(row, cols[col - 1], Piece(KNIGHT, current_colour, icon));
          break;
        case 3:
        case 6:
          icon = (current_colour == WHITE) ? "\u2657" : "\u265D";
          board[row - 1][col - 1] =
              Position(row, cols[col - 1], Piece(BISHOP, current_colour, icon));
          break;
        case 4:
          icon = (current_colour == WHITE) ? "\u2655" : "\u265B";
          board[row - 1][col - 1] =
              Position(row, cols[col - 1], Piece(QUEEN, current_colour, icon));
          break;
        case 5:
          icon = (current_colour == WHITE) ? "\u2654" : "\u265A";
          board[row - 1][col - 1] =
              Position(row, cols[col - 1], Piece(KING, current_colour, icon));
          break;
        default:
          break;
        }
      } else if (row == 2 || row == 7) {
        icon = (current_colour == WHITE) ? "\u2659" : "\u265F";
        board[row - 1][col - 1] =
            Position(row, cols[col], Piece(PAWN, current_colour, icon));
      } else {
        icon = " ";
        board[row - 1][col - 1] =
            Position(row, cols[col - 1], Piece(NO_PIECE, NO_COL, icon));
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

class Game {
  int move = 1;
  Colour next_to_move = WHITE;
  Board board{};
  Player player_1;
  Player player_2;

  Game(std::string name_1, std::string name_2)
      : player_1(WHITE, std::move(name_1)), player_2(BLACK, std::move(name_2)) {
  }
};

int main() {
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
  Board board{};
  std::string board_string = board.to_string();
  std::println("{}", board_string);

  return 0;
}