# CMakeList.txt : CMake project for AsyncRAII, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (AsyncRAII "main.cpp" "AsyncRAII.h" "AsyncRAII.cpp" "BufferedString.h" "IBuffer.h")

# target_link_libraries(AsyncRAII AsyncRAII)
