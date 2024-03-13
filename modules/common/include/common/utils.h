#pragma once

#include <string>
#include <vector>

namespace sudoku::common
{
  std::vector<std::string> ReadLinesFromFile(const std::string& p_path) noexcept;
} // namespace ninja::common
