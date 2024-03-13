#include "core/thread_worker.h"
#include "core/sudoku_context.h"

#include <iostream>
#include <unistd.h>
#include <deque>

#include <thread>
#include <atomic>

#include "common/game_type.h"
#include "common/utils.h"


void PrintHelp()
{
  std::cout << "Usage: $sudoku_solver -i $INPUT_FILE [-j $THREAD_COUNT]"<< std::endl;
}

struct ParsedParams
{
  std::string input_file {};
  uint32_t thread_num {std::thread::hardware_concurrency()};
};

ParsedParams ParseParams(int argc, char** argv)
{
  int opt;
  ParsedParams l_result;
  while ((opt = getopt(argc, argv, "i:j:h")) != -1)
  {
    switch (opt)
    {
      case 'h':
        PrintHelp();
        exit(0);
      case 'i':
        l_result.input_file = static_cast<std::string>(optarg); // Convert C-style string to std::string
        break;
      case 'j':
        try
        {
          l_result.thread_num = std::stoi(optarg);
        }
        catch (...)
        {

        }
        break;
      case '?': // Option not recognized
        PrintHelp();
        exit(1);
      default:
        break;
    }
  }

  if (l_result.input_file.empty())
  {
    PrintHelp();
    exit(2);
  }

  return l_result;
}
//-----------------------------------------------------
sudoku::common::Game InitializeGame(const std::string& file_path)
{
  auto lines = sudoku::common::ReadLinesFromFile(file_path);
  sudoku::common::Game game;
  uint8_t r_index = 0U;
  for (const auto& line : lines)
  {
    uint8_t col {0U};
    for (const auto& val : line)
    {
      uint8_t int_val = val-'0';
      if (int_val > 0)
      {
        game.set_value(r_index, col, int_val - 1);
      }
      ++col;
    }
    ++r_index;
  }

  return game;
}
//-----------------------------------------------------
int main(int argc, char** argv)
{
  auto params = ParseParams(argc, argv);
  auto game = InitializeGame(params.input_file);

  game.PrintMap();
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";

  sudoku::core::SudokuContext::game_tree.push_back(game);

  std::vector<std::thread> threads;
  for (int i = 0; i < params.thread_num; ++i)
  {
    threads.emplace_back(sudoku::core::worker_function);
  }

  for (auto& thread : threads)
  {
    thread.join();
  }

  if (!sudoku::core::SudokuContext::running)
  {
    sudoku::core::SudokuContext::solution.PrintMap();
  }

  return 0;
}
