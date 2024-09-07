set(EXECUTABLE_NAME runhw02)

add_library(hw02 combinatorics.cpp)

target_include_directories(hw02 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${EXECUTABLE_NAME} run.cpp)

target_link_libraries(${EXECUTABLE_NAME} hw02)

