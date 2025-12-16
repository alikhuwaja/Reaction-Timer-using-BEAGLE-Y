# CMake Build Configuration for root of project
cmake_minimum_required(VERSION 3.18)
project(ReactionTimer VERSION 1.0 DESCRIPTION "Starter project" LANGUAGES C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Werror -Wpedantic -Wextra -fdiagnostics-color)


add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# Enable pthread
add_compile_options(-pthread)
add_link_options(-pthread)


add_subdirectory(hal)
add_subdirectory(app)

