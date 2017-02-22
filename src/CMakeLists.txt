set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(target fast)

file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "*.hpp")

# Add binary build target
add_executable(${target} ${sources} ${headers})
target_link_libraries(${target} pugixml)

# Copy files to easy debug
file(COPY FIX50SP2-2017-Mar.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
