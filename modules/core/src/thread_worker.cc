#include "core/thread_worker.h"

#include "core/sudoku_context.h"

#include <iostream>
#include <thread>

namespace sudoku::core
{
  void worker_function()
  {
    std::cout << "Thread started [" <<std::this_thread::get_id() << "]" <<std::endl;

    while (SudokuContext::running)
    {
      std::unique_lock<std::mutex> lock(SudokuContext::game_tree_mutex);
      SudokuContext::game_tree_cv.wait(lock, [] { return !SudokuContext::game_tree.empty() || !SudokuContext::running; });

      if (!SudokuContext::running) break;


      auto game = SudokuContext::game_tree.back();
      SudokuContext::game_tree.pop_back();
      lock.unlock();

      if (not game.Solvable())
        continue;

      game.DoDeterministicSteps();

      if (game.Finished())
      {
        std::lock_guard<std::mutex> guard(SudokuContext::game_tree_mutex);
        SudokuContext::solution = game;
        SudokuContext::running = false;
        SudokuContext::game_tree_cv.notify_all();
      }
      else
      {
        auto [pos_x, pos_y, values] = game.FindFewestFreedomDegrees();
        for (const auto& value : values) {
          sudoku::common::Game game_opt = game;
          game_opt.set_value(pos_x, pos_y, value);
          std::lock_guard<std::mutex> guard(SudokuContext::game_tree_mutex);
          SudokuContext::game_tree.push_back(game_opt);
        }
        SudokuContext::game_tree_cv.notify_one();
      }
    }
  }
} // namespace sudoku::core
