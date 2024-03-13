set(sources
        src/thread_worker.cc
        src/sudoku_context.cc
        )

add_library(sudoku_core SHARED ${sources})

target_include_directories(sudoku_core
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

target_link_libraries(sudoku_core sudoku_common)