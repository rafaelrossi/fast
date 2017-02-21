cmake_minimum_required(VERSION 3.0)

# project name
project(fast)

# dependencies
add_subdirectory(deps/pugixml)

# build app
add_subdirectory(src)
