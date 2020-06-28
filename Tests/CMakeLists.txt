add_executable(SimpleArgsParserTests Main.cpp SimpleArgsParserTests.cpp)

target_link_libraries(SimpleArgsParserTests gtest SimpleArgsParser)
target_compile_options(SimpleArgsParserTests PRIVATE -std=c++17 -Wextra -Werror -Wall)

add_test(SimpleArgsParserTests SimpleArgsParserTests)
