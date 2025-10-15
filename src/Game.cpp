#include "Game.hpp"

#include <array>
#include <cctype>
#include <iostream>
#include <map>
#include <string>

#include "Piece.hpp"

using namespace chess;

std::map<unsigned, unsigned> rank_to_index = {{1, 0}, {2, 1}, {3, 2}, {4, 3},
                                              {5, 4}, {6, 5}, {7, 6}, {8, 7}};

std::map<std::string, unsigned> file_to_index = {{"a", 0}, {"b", 1}, {"c", 2},
                                                 {"d", 3}, {"e", 4}, {"f", 5},
                                                 {"g", 6}, {"h", 7}};

chess::BoardContext Game::make_board_context() {
  chess::BoardContext board_context{};
  for (const auto& [rank, row_index] : rank_to_index) {
    Colour current_colour = (row_index <= 1) ? WHITE : BLACK;
    for (const auto& [file, column_index] : file_to_index) {
      if (row_index == 0 || row_index == 7) {
        switch (column_index) {
          case 0:
          case 7:
            board_context.board[row_index][column_index] =
                Position(row_index, column_index,
                         std::make_unique<Rook>(current_colour));
            break;
          case 1:
          case 6:
            board_context.board[row_index][column_index] =
                Position(row_index, column_index,
                         std::make_unique<Knight>(current_colour));
            break;
          case 2:
          case 5:
            board_context.board[row_index][column_index] =
                Position(row_index, column_index,
                         std::make_unique<Bishop>(current_colour));
            break;
          case 3:
            board_context.board[row_index][column_index] =
                Position(row_index, column_index,
                         std::make_unique<Queen>(current_colour));
            break;
          case 4:
            board_context.board[row_index][column_index] =
                Position(row_index, column_index,
                         std::make_unique<King>(current_colour));
            break;
          default:
            break;
        }
      } else if (row_index == 1 || row_index == 6) {
        board_context.board[row_index][column_index] = Position(
            row_index, column_index, std::make_unique<Pawn>(current_colour));
      } else {
        board_context.board[row_index][column_index] =
            Position(row_index, column_index, nullptr);
      }
    }
    board_context.ptr_to_white_king_position = &board_context.board[0][4];
    board_context.ptr_to_white_king_position = &board_context.board[0][4];
  }
  return board_context;
}

std::string Game::to_string() {
  if (this->board_context_ptr) {
    std::string result = std::string("+------------------------+\n");
    for (const auto& [rank, row_index] : rank_to_index) {
      result += std::string("|");
      for (const auto& [file, column_index] : file_to_index) {
        Piece* piece_ptr =
            (*board_context_ptr).board[row_index][column_index].get_piece_ptr();
        if (piece_ptr) {
          result += "\u200A" + piece_ptr->get_icon() + "\u200A";
        } else {
          result += std::string("\u200A") + std::string("\u200A") +
                    std::string("\u200A");
        }
      }
      result += std::string("|\n");
    }
    result += std::string("+------------------------+\n");
    return result;
  }

  return "error: no board found.";
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
  auto it_start_col = file_to_index.find(start_key);
  if (it_start_col == file_to_index.end())
    return "Illegal input: invalid start column letter";
  start_col = it_start_col->second;

  char c2 = static_cast<char>(std::tolower(static_cast<unsigned char>(s[2])));
  std::string end_key(1, c2);
  auto it_end_col = file_to_index.find(end_key);
  if (it_end_col == file_to_index.end())
    return "Illegal input: invalid end column letter";
  end_col = it_end_col->second;

  if (!std::isdigit(static_cast<unsigned char>(s[1])) ||
      !std::isdigit(static_cast<unsigned char>(s[3])))
    return "Illegal input: invalid row characters";

  unsigned start_row_chess = s[1] - '0';
  unsigned end_row_chess = s[3] - '0';

  auto it_start_row = rank_to_index.find(start_row_chess);
  if (it_start_row == rank_to_index.end())
    return "Illegal input: invalid start row number";
  start_row = it_start_row->second;

  auto it_end_row = rank_to_index.find(end_row_chess);
  if (it_end_row == rank_to_index.end())
    return "Illegal input: invalid end row number";
  end_row = it_end_row->second;

  if (start_col > 7 || end_col > 7 || start_row > 7 || end_row > 7)
    return "Illegal input: column or row out of bounds";

  return "Legal input";
}

std::string request_and_execute_move(std::string player_name,
                                     std::string& player_input,
                                     unsigned& start_column,
                                     unsigned& start_row, unsigned& end_column,
                                     unsigned& end_row, Game& game) {
  std::println("{}, your move: ", player_name);
  std::cin >> player_input;
  std::string parse_info = parse_move_input(player_input, start_column,
                                            start_row, end_column, end_row);

  if (!game.get_board_context_ptr()) {
    std::println("error: board pointer is null");
  }

  if (parse_info.find("Illegal") != std::string::npos) {
    std::println("{}", parse_info);
    return request_and_execute_move(player_name, player_input, start_column,
                                    start_row, end_column, end_row, game);
  }
  // check that there's a piece at start_pos
  auto moved_piece_ptr = game.get_board_context_ptr()
                             ->board[start_row][start_column]
                             .get_piece_ptr();
  if (!(moved_piece_ptr)) {
    parse_info = "Illegal input: no piece at start position";
    std::println("{}", parse_info);
    return request_and_execute_move(player_name, player_input, start_column,
                                    start_row, end_column, end_row, game);
  }
  // check if piece of correct colour is tried to be moved
  Colour start_colour = moved_piece_ptr->get_Colour();
  if (start_colour != game.get_next_to_move()) {
    parse_info = "Illegal input: tried to move opponent's piece";
    std::println("{}", parse_info);
    return request_and_execute_move(player_name, player_input, start_column,
                                    start_row, end_column, end_row, game);
  }

  // verify start_pos != end_pos
  if (start_row == end_row && start_column == end_column) {
    parse_info = "Illegal input: start and end position are identical";
    std::println("{}", parse_info);
    return request_and_execute_move(player_name, player_input, start_column,
                                    start_row, end_column, end_row, game);
  }
  auto captured_piece_ptr =
      game.get_board_context_ptr()->board[end_row][end_column].get_piece_ptr();

  if (captured_piece_ptr) {
    Colour end_colour = captured_piece_ptr->get_Colour();
    // there is a piece at end_pos
    if (start_colour == end_colour) {
      // end_pos is occupied with piece of own colour
      parse_info = "Illegal move: you try to take your own piece";
      std::println("{}", parse_info);
      return request_and_execute_move(player_name, player_input, start_column,
                                      start_row, end_column, end_row, game);
    }
  }
  // try to perform the move
  parse_info = moved_piece_ptr->move(
      *game.get_board_context_ptr(),
      game.get_board_context_ptr()->board[start_row][start_column],
      game.get_board_context_ptr()->board[end_row][end_column],
      game.get_move_count());

  if (parse_info.find("Illegal") != std::string::npos) {
    std::println("{}", parse_info);
    return request_and_execute_move(player_name, player_input, start_column,
                                    start_row, end_column, end_row, game);
  }
  // Move was legal
  return parse_info;
}

void Game::play_game() {
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

  std::println(
      "Please enter moves using the following syntax: "
      "<start_column><start_row><end_column><end_row>");

  while (game.get_status() == ONGOING) {
    game.set_next_to_move(WHITE);
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
}