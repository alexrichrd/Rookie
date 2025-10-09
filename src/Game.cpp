#include "Game.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <string>

#include "Board.hpp"
#include "Piece.hpp"

using namespace chess;

Game::Game(std::string name_1, std::string name_2)
    : player_1(WHITE, std::move(name_1)),
      player_2(BLACK, std::move(name_2)),
      board_ptr(std::make_unique<Board>()) {};
std::string Game::get_board() { return this->board_ptr->to_string(); }
GameStatus Game::get_status() { return this->status; }
std::string Game::get_player_name(const unsigned player_number) {
  if (player_number == 1) {
    return this->player_1.get_name();
  }
  return this->player_2.get_name();
}

std::string parse_move_input(std::string& input, char& start_col,
                             unsigned& start_row, char& end_col,
                             unsigned& end_row) {
  std::string s = input;
  s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

  s.erase(std::remove(s.begin(), s.end(), '('), s.end());
  s.erase(std::remove(s.begin(), s.end(), ')'), s.end());
  s.erase(std::remove(s.begin(), s.end(), ','), s.end());

  if (s.size() != 4) return "error: input string must have length four";

  start_col = std::tolower(s[0]);
  start_row = s[1] - '0';
  end_col = std::tolower(s[2]);
  end_row = s[3] - '0';

  if (start_col < 'a' || start_col > 'h' || end_col < 'a' || end_col > 'h' ||
      start_row < 1 || start_row > 8 || end_row < 1 || end_row > 8)
    return "error: column or row out of bounds";

  return std::to_string(start_col) + std::to_string(start_row) +
         std::to_string(end_col) + std::to_string(end_row);
}

std::string request_and_execute_move(std::string player_name,
                                     std::string& player_input, char& start_col,
                                     unsigned& start_row, char& end_col,
                                     unsigned& end_row, Game& game) {
  std::println("{}, your move (<start_col><start_row><end_col><end_row>): ",
               player_name);
  std::cin >> player_input;
  std::string parse_info =
      parse_move_input(player_input, start_col, start_row, end_col, end_row);

  if (!game.get_board_ptr()) {
    std::println("board pointer is null");
  }

  if (parse_info.find("error") != std::string::npos) {
    parse_info = "no piece at start position";
    std::println("{}, that was an illegal move ({}).", player_name, parse_info);
    request_and_execute_move(player_name, player_input, start_col, start_row,
                             end_col, end_row, game);
  }
  // check that there's a piece at start_pos
  if (!game.get_board_ptr()->board[start_row][start_col].get_raw_piece_ptr()) {
    parse_info = "no piece at start position";
    std::println("{}, that was an illegal move ({}).", player_name, parse_info);
    request_and_execute_move(player_name, player_input, start_col, start_row,
                             end_col, end_row, game);
  }
  // verify start_pos != end_pos
  if (start_row == end_row && start_col == end_col) {
    parse_info = "start and end position are identical";
    std::println("{}, that was an illegal move ({}).", player_name, parse_info);
    request_and_execute_move(player_name, player_input, start_col, start_row,
                             end_col, end_row, game);
  }
  if (game.get_board_ptr()->board[end_row][end_col].get_raw_piece_ptr()) {
    Colour start_colour = game.get_board_ptr()
                              ->board[start_row][start_col]
                              .get_raw_piece_ptr()
                              ->get_Colour();
    Colour end_colour = game.get_board_ptr()
                            ->board[end_row][end_col]
                            .get_raw_piece_ptr()
                            ->get_Colour();
    // there is a piece at end_pos
    if (start_colour == end_colour) {
      // end_pos is occupied with piece of own colour
      parse_info = "end position contains piece of own colour";
      std::println("{}, that was an illegal move ({}).", player_name,
                   parse_info);
      request_and_execute_move(player_name, player_input, start_col, start_row,
                               end_col, end_row, game);
    }
  }
  // perform the move
  game.get_board_ptr()->board[start_row][start_col].get_raw_piece_ptr()->move(
      game.get_board_ptr(), game.get_board_ptr()->board[start_row][start_col],
      game.get_board_ptr()->board[end_row][end_col]);

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
  std::println("{}", game.get_board());
  std::string player_input;
  unsigned start_row;
  char start_col;
  unsigned end_row;
  char end_col;
  std::string player_white_output;
  std::string player_black_output;

  while (game.get_status() == ONGOING) {
    player_white_output = request_and_execute_move(
        name_1, player_input, start_col, start_row, end_col, end_row, game);
    player_black_output = request_and_execute_move(
        name_2, player_input, start_col, start_row, end_col, end_row, game);
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