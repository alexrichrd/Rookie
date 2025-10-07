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
      board_ptr(std::make_unique<Board>(Board())) {}
std::string Game::get_board() { return this->board_ptr->to_string(); }
GameStatus Game::get_status() { return this->status; }
std::string Game::get_player_name(const unsigned player_number) {
  if (player_number == 1) {
    return this->player_1.get_name();
  }
  return this->player_2.get_name();
}

bool parse_move_input(std::string& input, char& start_col, unsigned& start_row,
                      char& end_col, unsigned& end_row) {
  std::string s = input;
  s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

  s.erase(std::remove(s.begin(), s.end(), '('), s.end());
  s.erase(std::remove(s.begin(), s.end(), ')'), s.end());
  s.erase(std::remove(s.begin(), s.end(), ','), s.end());

  if (s.size() != 4) return false;

  start_col = std::tolower(s[0]);
  start_row = s[1] - '0';
  end_col = std::tolower(s[2]);
  end_row = s[3] - '0';

  if (start_col < 'a' || start_col > 'h' || end_col < 'a' || end_col > 'h' ||
      start_row < 1 || start_row > 8 || end_row < 1 || end_row > 8)
    return false;

  return true;
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
  // variables for white's moves
  std::string player1_input;
  unsigned start_row_white;
  char start_col_white;
  unsigned end_row_white;
  char end_col_white;
  bool successful_player1_parse;
  // variables for black's moves
  std::string player2_input;
  // unsigned start_row_black;
  // char start_col_black;
  // unsigned end_row_black;
  // char end_col_black;

  while (game.get_status() == ONGOING) {
    std::println("{}, your move (<start_col><start_row><end_col><end_row>): ",
                 name_1);
    std::cin >> player1_input;
    successful_player1_parse =
        parse_move_input(player1_input, start_col_white, start_row_white,
                         end_col_white, end_row_white);
    if (successful_player1_parse) {
      game.get_board_ptr()
          ->board[start_row_white][start_col_white]
          .get_raw_piece_ptr()
          ->move(game.get_board_ptr(),
                 &game.get_board_ptr()->board[start_row_white][start_col_white],
                 &game.get_board_ptr()->board[end_row_white][end_col_white]);
    } else {
      do {
        std::println("{}, that was an illegal move. Again please: ", name_1);
        std::cin >> player1_input;
        successful_player1_parse =
            parse_move_input(player1_input, start_col_white, start_row_white,
                             end_col_white, end_row_white);
      } while (!successful_player1_parse);
    }
    // player 2 turn

    /* make moves */
    // // check that there's a piece at start_pos
    // if (!start_pos->get_raw_piece_ptr()) {
    // return false;
    // }
    // // verify start_pos != end_pos
    // if (start_row == end_row && start_col == end_col) {
    // return false;
    // }
    //     if (end_pos->get_raw_piece_ptr()) {
    //   // there is a piece at end_pos
    //   if (start_pos->get_raw_piece_ptr()->get_Colour() ==
    //     end_pos->get_raw_piece_ptr()->get_Colour()) {
    //     // check end_pos is not occupied with piece of own colour
    //     return false;
    //   }
    // }
    break;
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