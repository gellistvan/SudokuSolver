#pragma once
#include "common/game_type.h"

#include <deque>
#include <condition_variable>
#include <atomic>

namespace sudoku::core
{
  struct SudokuContext
  {
    static std::deque<sudoku::common::Game> game_tree;
    static std::mutex game_tree_mutex;
    static std::condition_variable game_tree_cv;
    static std::atomic<bool> running;
    static sudoku::common::Game solution;
  };
} // namespace sudoku::core
