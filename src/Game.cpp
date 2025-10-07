#include "Game.hpp"

#include <array>
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
  while (game.get_status() == ONGOING) {
    /* make moves */
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