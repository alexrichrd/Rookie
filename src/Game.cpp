#include "Game.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <map>
#include <string>

#include "Piece.hpp"

using namespace chess;

std::map<std::string, unsigned> chessCol_to_arrayCol = {
    {"a", 0}, {"b", 1}, {"c", 2}, {"d", 3},
    {"e", 4}, {"f", 5}, {"g", 6}, {"h", 7}};

std::map<unsigned, unsigned> chessRow_to_arrayRow = {
    {1, 0}, {2, 1}, {3, 2}, {4, 3}, {5, 4}, {6, 5}, {7, 6}, {8, 7}};

std::array<std::array<Position, 8>, 8> Game::makeBoard() {
  std::array<std::array<Position, 8>, 8> board{};
  for (const auto& [chessRow, arrayRow] : chessRow_to_arrayRow) {
    Colour current_colour = (arrayRow <= 1) ? WHITE : BLACK;
    for (const auto& [chessCol, arrayCol] : chessCol_to_arrayCol) {
      if (arrayRow == 0 || arrayRow == 7) {
        switch (arrayCol) {
          case 0:
          case 7:
            board[arrayRow][arrayCol] = Position(
                arrayRow, arrayCol, std::make_unique<Rook>(current_colour));
            std::println("col {}, row {}, colour {}, Rook", arrayCol, arrayRow,
                         ColourToString(current_colour));
            break;
          case 1:
          case 6:
            board[arrayRow][arrayCol] = Position(
                arrayRow, arrayCol, std::make_unique<Knight>(current_colour));
            std::println("col {}, row {}, colour {}, Knight", arrayCol,
                         arrayRow, ColourToString(current_colour));
            break;
          case 2:
          case 5:
            board[arrayRow][arrayCol] = Position(
                arrayRow, arrayCol, std::make_unique<Bishop>(current_colour));
            std::println("col {}, row {}, colour {}, Bishop", arrayCol,
                         arrayRow, ColourToString(current_colour));
            break;
          case 3:
            board[arrayRow][arrayCol] = Position(
                arrayRow, arrayCol, std::make_unique<Queen>(current_colour));
            std::println("col {}, row {}, colour {}, Queen", arrayCol, arrayRow,
                         ColourToString(current_colour));
            break;
          case 4:
            board[arrayRow][arrayCol] = Position(
                arrayRow, arrayCol, std::make_unique<King>(current_colour));
            std::println("col {}, row {}, colour {}, King", arrayCol, arrayRow,
                         ColourToString(current_colour));

            break;
          default:
            break;
        }
      } else if (arrayRow == 1 || arrayRow == 6) {
        board[arrayRow][arrayCol] = Position(
            arrayRow, arrayCol, std::make_unique<Pawn>(current_colour));
        std::println("col {}, row {}, colour {}, Pawn", arrayCol, arrayRow,
                     ColourToString(current_colour));

      } else {
        board[arrayRow][arrayCol] = Position(arrayRow, arrayCol, nullptr);
      }
    }
  }
  return board;
}

std::string Game::to_string() {
  if (this->board_ptr) {
    std::string result = std::string("+------------------------+\n");
    for (const auto& [chessRow, arrayRow] : chessRow_to_arrayRow) {
      result += std::string("|");
      for (const auto& [chessCol, arrayCol] : chessCol_to_arrayCol) {
        if ((*board_ptr)[arrayRow][arrayCol].get_piece_ptr()) {
          result +=
              "\u200A" +
              (*board_ptr)[arrayRow][arrayCol].get_piece_ptr()->get_icon() +
              "\u200A";
        } else {
          result += std::string("\u200A") + std::string("\u200A") +
                    std::string("\u200A");
        }
      }
      result += std::string("|\n");
    }
    result += std::string("+------------------------+\n");
    return result;
  } else {
    return "error: no board found.";
  }
}

Game::Game(std::string name_1, std::string name_2)
    : player_1(WHITE, std::move(name_1)),
      player_2(BLACK, std::move(name_2)),
      board_ptr(std::make_unique<Board>(makeBoard())) {};

GameStatus Game::get_status() { return this->status; }
std::string Game::get_player_name(const unsigned player_number) {
  if (player_number == 1) {
    return this->player_1.get_name();
  }
  return this->player_2.get_name();
}

std::string parse_move_input(std::string& input, unsigned& start_col,
                             unsigned& start_row, unsigned& end_col,
                             unsigned& end_row) {
  std::string s = input;
  s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
  s.erase(std::remove(s.begin(), s.end(), '('), s.end());
  s.erase(std::remove(s.begin(), s.end(), ')'), s.end());
  s.erase(std::remove(s.begin(), s.end(), ','), s.end());

  if (s.size() != 4) return "Illegal input: input string must have length four";

  char c0 = static_cast<char>(std::tolower(static_cast<unsigned char>(s[0])));
  std::string start_key(1, c0);
  auto it_start_col = chessCol_to_arrayCol.find(start_key);
  if (it_start_col == chessCol_to_arrayCol.end())
    return "Illegal input: invalid start column letter";
  start_col = it_start_col->second;

  char c2 = static_cast<char>(std::tolower(static_cast<unsigned char>(s[2])));
  std::string end_key(1, c2);
  auto it_end_col = chessCol_to_arrayCol.find(end_key);
  if (it_end_col == chessCol_to_arrayCol.end())
    return "Illegal input: invalid end column letter";
  end_col = it_end_col->second;

  if (!std::isdigit(static_cast<unsigned char>(s[1])) ||
      !std::isdigit(static_cast<unsigned char>(s[3])))
    return "Illegal input: invalid row characters";

  unsigned start_row_chess = s[1] - '0';
  unsigned end_row_chess = s[3] - '0';

  auto it_start_row = chessRow_to_arrayRow.find(start_row_chess);
  if (it_start_row == chessRow_to_arrayRow.end())
    return "Illegal input: invalid start row number";
  start_row = it_start_row->second;

  auto it_end_row = chessRow_to_arrayRow.find(end_row_chess);
  if (it_end_row == chessRow_to_arrayRow.end())
    return "Illegal input: invalid end row number";
  end_row = it_end_row->second;

  if (start_col > 7 || end_col > 7 || start_row > 7 || end_row > 7)
    return "Illegal input: column or row out of bounds";

  return "Legal input";
}

std::string request_and_execute_move(std::string player_name,
                                     std::string& player_input,
                                     unsigned& start_col, unsigned& start_row,
                                     unsigned& end_col, unsigned& end_row,
                                     Game& game) {
  // TODO: check if its your turn to move 
  std::println("{}, your move (<start_col><start_row><end_col><end_row>): ",
               player_name);
  std::cin >> player_input;
  std::string parse_info =
      parse_move_input(player_input, start_col, start_row, end_col, end_row);

  if (!game.get_board_ptr()) {
    std::println("error: board pointer is null");
  }

  if (parse_info.find("Illegal") != std::string::npos) {
    std::println("{}", parse_info);
    return request_and_execute_move(player_name, player_input, start_col, start_row,
                             end_col, end_row, game);
  }
  // check that there's a piece at start_pos
  if (!(game.get_board_ptr()->at(start_row).at(start_col).get_piece_ptr())) {
    parse_info = "Illegal input: no piece at start position";
    std::println("{}", parse_info);
    return request_and_execute_move(player_name, player_input, start_col, start_row,
                             end_col, end_row, game);
  }
  // verify start_pos != end_pos
  if (start_row == end_row && start_col == end_col) {
    parse_info = "Illegal input: start and end position are identical";
    std::println("{}", parse_info);
    return request_and_execute_move(player_name, player_input, start_col, start_row,
                             end_col, end_row, game);
  }
  if (game.get_board_ptr()->at(end_row).at(end_col).get_piece_ptr()) {
    Colour start_colour = game.get_board_ptr()
                              ->at(start_row)
                              .at(start_col)
                              .get_piece_ptr()
                              ->get_Colour();
    Colour end_colour = game.get_board_ptr()
                            ->at(end_row)
                            .at(end_col)
                            .get_piece_ptr()
                            ->get_Colour();
    // there is a piece at end_pos
    if (start_colour == end_colour) {
      // end_pos is occupied with piece of own colour
      parse_info = "Illegal move: you try to take your own piece";
      std::println("{}", parse_info);
      return request_and_execute_move(player_name, player_input, start_col, start_row,
                               end_col, end_row, game);
    }
  }

  std::println("Move: {}{} to {}{}", start_col, start_row, end_col, end_row);
  // try to perform the move
  parse_info = game.get_board_ptr()->at(start_row).at(start_col).get_piece_ptr()->move(
      *game.get_board_ptr(), game.get_board_ptr()->at(start_row).at(start_col),
      game.get_board_ptr()->at(end_row).at(end_col));

  if (parse_info.find("Illegal") != std::string::npos) {
    std::println("{}", parse_info);
    return request_and_execute_move(player_name, player_input, start_col, start_row,
                             end_col, end_row, game);
  }
  // Move was legal
  return parse_info;
}

int main() {
  // Get player names
  std::string name_1, name_2;
  std::println("Welcome! Player 1, please enter your name:");
  std::cin >> name_1;
  std::println("Player 2, please enter your name:");
  std::cin >> name_2;
  while (name_2 == name_1) {
    std::println(
        "Player names must not be identical. Player 2, please enter a "
        "new name:");
    std::cin >> name_2;
  }

  // Initialize game
  Game game(name_1, name_2);
  std::println("{}", game.to_string());
  std::string player_input;
  // the parsed player input goes here:
  unsigned start_row;
  unsigned start_col;
  unsigned end_row;
  unsigned end_col;
  std::string player_white_output;
  std::string player_black_output;

  // auto piece_ptr = game.get_board_ptr()->at(0).at(0).get_piece_ptr();
  // if (piece_ptr) {
  //   std::println("{}", ColourToString(piece_ptr->get_Colour()));
  // } else {
  //   std::println("Empty square");
  // }

  while (game.get_status() == ONGOING) {
    player_white_output = request_and_execute_move(
        name_1, player_input, start_col, start_row, end_col, end_row, game);
    std::println("{}", game.to_string());
    game.set_next_to_move(BLACK);
    player_black_output = request_and_execute_move(
        name_2, player_input, start_col, start_row, end_col, end_row, game);
    std::println("{}", game.to_string());
    game.add_move(
        {player_white_output, player_black_output, game.get_move_count()});
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