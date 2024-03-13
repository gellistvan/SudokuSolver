#pragma once

#include "basic_types.h"
#include <cstdint>
#include <limits>

#include <array>
#include <bitset>
#include <variant>

namespace sudoku::common
{
  struct FieldType
  {
    std::variant<std::bitset<9U>, CoordinateType> data = std::bitset<9U>(511U);

    bool is_secured () const
    {
      return std::holds_alternative<CoordinateType>(data);
    }

    CoordinateType GetValue() const
    {
      return std::get<CoordinateType>(data);
    }

    bool IsEmpty () const
    {
      return not is_secured();
    }

    void DisableValue(CoordinateType value)
    {
      auto& bitset_ = std::get<std::bitset<9u>>(data);
      bitset_.set(value, false);
    }

    void Enabled(uint8_t value)
    {
      auto& bitset_ = std::get<std::bitset<9u>>(data);
      bitset_.test(value);
    }

    std::bitset<9>& GetOptions()
    {
      return std::get<std::bitset<9u>>(data);
    }
    const std::bitset<9>& GetOptions() const
    {
      return std::get<std::bitset<9u>>(data);
    }
  };
} // namespace sudoku::common
