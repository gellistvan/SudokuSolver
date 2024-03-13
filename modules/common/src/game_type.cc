#include "common/game_type.h"

#include <vector>
#include <tuple>
#include <iostream>

#include <map>
#include <type_traits>


namespace sudoku::common
{
  void Game::set_value(uint8_t r_index, uint8_t c_index, CoordinateType value)
  {
    data[r_index][c_index].data = value;

    uint8_t sq_x = r_index/3;
    uint8_t sq_y = c_index/3;

    for (uint8_t x = 0U; x < 9U; ++x)
    {
      uint8_t SQ_x = x/3;
      for (uint8_t y = 0U; y < 9U; ++y)
      {
        auto& field = data[x][y];
        if (field.is_secured())
        {
          continue;
        }
        if (x == r_index or y == c_index or (SQ_x == sq_x and y/3 == sq_y))
        {
          field.DisableValue(value);
        }
      }
    }
  }

  void Game::PrintMap()
  {
    for (uint8_t r_index = 0U; r_index < 9U; ++r_index)
    {
      if (0U == r_index % 3U)
      {
        std::cout << "+---+---+---+\n";
      }

      const auto& row = data[r_index];
      for (uint8_t c_index = 0U; c_index < 9U; ++c_index)
      {
        if (0U == c_index % 3U )
        {
          std::cout << "|";
        }

        if ( row[c_index].is_secured())
          std::cout << std::to_string(row[c_index].GetValue() + 1);
        else std::cout << " ";
      }
      std::cout << "|\n";
    }
    std::cout << "+---+---+---+\n";
  }

  bool Game::CheckSecured()
  {
    bool changed = false;
    for (uint8_t r_index = 0U; r_index < 9U; ++r_index)
    {
      for (uint8_t c_index = 0U; c_index < 9U; ++c_index)
      {
        if (data[r_index][c_index].is_secured())
        {
          continue;
        }

        auto& options = data[r_index][c_index].GetOptions();
        if (options.count() == 1)
        {
          CoordinateType value =  options._Find_first();
          set_value(r_index, c_index, value);
          changed = true;
        }
      }
    }

    return changed;
  }

  bool Game::CheckCoordinates(const std::vector<std::pair<uint8_t, uint8_t>>& p_coordinates )
  {
    bool changed = false;
    std::array<uint8_t, 9U> counts {};
    std::map<uint8_t, std::pair<uint8_t, uint8_t>> first_pos;

    for (const auto& [row, col] : p_coordinates)
    {
      if(data[row][col].is_secured())
      {
        counts[data[row][col].GetValue()] = 0;
        continue;
      }
      const auto& options = data[row][col].GetOptions();
      std::string bitstring = options.to_string();

      for (uint8_t value = 0U; value < 9U; ++value)
      {
        if(options.test(value))
        {
          counts[value]++;
          first_pos[value] = { row, col };
        }
      }
    }

    for (uint8_t value = 0U; value < 9U; ++value)
    {
      if(counts[value] == 1)
      {
        changed = true;
        set_value(first_pos[value].first, first_pos[value].second, value);
      }
    }


    return changed;
  }

  bool Game::CheckRows()
  {
    bool changed = false;
    std::vector<std::pair<uint8_t, uint8_t>> row_coordinates {};
    for (uint8_t r_index = 0U; r_index < 9U; ++r_index)
    {
      for (uint8_t c_index = 0U; c_index < 9U; ++c_index)
      {
        row_coordinates.emplace_back(std::make_pair(r_index, c_index));
      }

      if (CheckCoordinates(row_coordinates) and not changed)
      {
        changed = true;
      }
    }
    return changed;
  }

  bool Game::CheckCols()
  {
    bool changed = false;
    std::vector<std::pair<uint8_t, uint8_t>> row_coordinates {};
    for (uint8_t c_index = 0U; c_index < 9U; ++c_index)
    {
      for (uint8_t r_index = 0U; r_index < 9U; ++r_index)
      {
        row_coordinates.emplace_back(std::make_pair(r_index, c_index));
      }

      if (CheckCoordinates(row_coordinates) and not changed)
      {
        changed = true;
      }
    }
    return changed;
  }

  bool Game::CheckSqueres()
  {
    bool changed = false;
    for (uint8_t y : {0U, 1U, 2U})
    {
      for (uint8_t x : {0U, 1U, 2U})
      {
        std::vector<std::pair<uint8_t, uint8_t>> squere_coordinates{
            { y * 3 + 0U, x * 3 + 0U}, { y * 3 + 1U, x * 3 + 0U }, { y * 3 + 2U, x * 3 + 0U },
            { y * 3 + 0U, x * 3 + 1U}, { y * 3 + 1U, x * 3 + 1U }, { y * 3 + 2U, x * 3 + 1U },
            { y * 3 + 0U, x * 3 + 2U}, { y * 3 + 1U, x * 3 + 2U }, { y * 3 + 2U, x * 3 + 2U },
        };

        if (CheckCoordinates(squere_coordinates) and not changed)
        {
          changed = true;
        }
      }
    }
    return changed;

  }

  bool Game::Finished() const
  {
    for (const auto& row : data)
    {
      for (const auto& field : row)
      {
        if (not field.is_secured())
        {
          return false;
        }
      }
    }
    return true;
  }

  bool Game::Solvable () const
  {
    for (const auto& row : data)
    {
      for (const auto& field : row)
      {
        if (not field.is_secured() and field.GetOptions().count() == 0)
        {
          return false;
        }
      }
    }
    return true;
  }

  void Game::DoDeterministicSteps()
  {
    bool changed;
    do
    {
      changed = CheckSecured();
      if (changed)
      {
        continue;
      }

      changed = CheckRows();
      if (changed)
      {
        continue;
      }

      changed = CheckCols();
      if (changed)
      {
        continue;
      }

      changed = CheckSqueres();
      if (changed)
      {
        continue;
      }

    } while(changed);
  }

  std::tuple<CoordinateType, CoordinateType, std::vector<CoordinateType>> Game::FindFewestFreedomDegrees() const
  {
    CoordinateType min_x, min_y;
    std::vector <CoordinateType> options{};
    for (CoordinateType r_pos = 0U; r_pos < 9U; ++r_pos)
    {
      for (CoordinateType c_pos = 0U; c_pos < 9U; ++c_pos)
      {
        if (data[r_pos][c_pos].is_secured())
          continue;

        const auto& field_options = data[r_pos][c_pos].GetOptions();

        if (field_options.count() < options.size() or options.empty())
        {
          options.clear();
          min_x = r_pos;
          min_y = c_pos;
          for (CoordinateType value = 0U; value < 9U; ++value)
          {
            if (field_options.test(value))
            {
              options.push_back(value);
            }
          }
        }
      }
    }

    return { min_x, min_y, options };
  }
} // namespace sudoku::common