#include "core/sudoku_context.h"

namespace sudoku::core
{
  std::deque<sudoku::common::Game> SudokuContext::game_tree = {};
  std::mutex SudokuContext::game_tree_mutex = {};
  std::condition_variable SudokuContext::game_tree_cv = {};
  std::atomic<bool> SudokuContext::running = true;
  sudoku::common::Game SudokuContext::solution = {};
} // namespace sudoku::core
