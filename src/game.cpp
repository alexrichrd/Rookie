#include <array>
#include <iostream>
#include <print>
#include <string>

enum PieceType {
  PAWN,
  ROOK,
  KNIGHT,
  BISHOP,
  QUEEN,
  KING,
  NO_PIECE
};

enum Colour { WHITE, BLACK, NO_COL };

class Piece {
public:
  PieceType piece_type;
  Colour colour;
  std::string icon;

public:
  Piece(PieceType piece_type, Colour colour, std::string icon)
      : piece_type(piece_type), colour(colour), icon(icon) {}
};

class Position {
  char row;
  int col;
  Piece piece{NO_PIECE, NO_COL, " "};

public:
  Position() {};
  Position(char row, int col, Piece piece) : row(row), col(col), piece(piece) {}
  std::string get_icon() { return piece.icon; }
};

class Board {
  std::array<std::array<Position, 8>, 8> board;

  static std::array<std::array<Position, 8>, 8> makeBoard() {
    std::array<std::array<Position, 8>, 8> board{};
    char cols[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    int rows[8] = {1, 2, 3, 4, 5, 6, 7, 8};
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
            board[row - 1][col - 1] = Position(
                row, cols[col - 1], Piece(KNIGHT, current_colour, icon));
            break;
          case 3:
          case 6:
            icon = (current_colour == WHITE) ? "\u2657" : "\u265D";
            board[row - 1][col - 1] = Position(
                row, cols[col - 1], Piece(BISHOP, current_colour, icon));
            break;
          case 4:
            icon = (current_colour == WHITE) ? "\u2655" : "\u265B";
            board[row - 1][col - 1] = Position(
                row, cols[col - 1], Piece(QUEEN, current_colour, icon));
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

public:
  Board() : board(makeBoard()) {};
  std::string to_string() {
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
};

class Player {
  Colour colour;
  std::string name;

public:
  Player(Colour colour, std::string name) : colour(colour), name(name) {}
};

class Game {
  int move = 1;
  Colour next_to_move = WHITE;
  Board board{};
  Player player_1;
  Player player_2;

  Game(std::string name_1, std::string name_2)
      : player_1(WHITE, name_1), player_2(BLACK, name_2) {}
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