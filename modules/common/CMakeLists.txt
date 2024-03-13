set(sources
        src/game_type.cc
        src/utils.cc
        )

add_library(sudoku_common SHARED ${sources})

target_include_directories(sudoku_common
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )
