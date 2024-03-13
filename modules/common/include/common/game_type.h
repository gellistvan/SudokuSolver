#pragma once

#include "field_type.h"

#include <vector>
#include <tuple>

namespace sudoku::common
{
  class Game
  {
  public:
    void set_value(uint8_t r_index, uint8_t c_index, CoordinateType value);

    void PrintMap();
    [[nodiscard]] bool Finished() const;
    [[nodiscard]] bool Solvable() const;
    void DoDeterministicSteps();

    [[nodiscard]] std::tuple<CoordinateType, CoordinateType, std::vector<CoordinateType>> FindFewestFreedomDegrees() const;

  private:
    bool CheckSecured();
    bool CheckCoordinates(const std::vector<std::pair<uint8_t, uint8_t>>& p_coordinates );
    bool CheckRows();
    bool CheckCols();
    bool CheckSqueres();

    std::array<std::array<FieldType, 9U>, 9U> data {};
  };
} // namespace sudoku::common
