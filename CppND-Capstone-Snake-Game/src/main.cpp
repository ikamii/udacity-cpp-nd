#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "highscore.h"
#include "player.h"
#include "replay.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Replay replay;

  while (replay.playAgain()) {

      HighScore highScore;
      Player player;

      std::cout << "Enter your name: ";
      std::string name;
      std::cin >> name;
      player.setName(name);


      Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
      Controller controller;
      Game game(kGridWidth, kGridHeight);
      game.Run(controller, renderer, kMsPerFrame);
      std::cout << "Game has terminated successfully!\n";
      std::cout << "Score: " << game.GetScore() << "\n";
      std::cout << "Size: " << game.GetSize() << "\n\n";

      highScore.Read();
      highScore.Update(player.getName(), game.GetScore());
      highScore.Write();
      highScore.Print();

      replay.gameContinue();
  }

  return 0;
}