#include "common/utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace sudoku::common
{
  std::vector<std::string> ReadLinesFromFile(const std::string& p_path) noexcept
  {
    if (!fs::exists(p_path))
    {
      std::cerr << "Input file does not exist " << p_path << std::endl;
      return {};
    }

    std::vector <std::string> map_files{};
    std::ifstream input_file (p_path, std::ifstream::in);
    try
    {
      if (!input_file.is_open())
      {
        std::cerr << "Could not open input file: " << p_path << std::endl;
        return {};
      }

      std::string line{};
      while (std::getline(input_file, line))
      {
        map_files.push_back(line);
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << "IO error while reading file: " << p_path << std::endl;
      return {};
    }

    return map_files;
  }
} // namespace ninja::common
