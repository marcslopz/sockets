cmake_minimum_required(VERSION 2.6)
project(sockets)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

add_executable(
  minimum_thread_server
  src/minimum_thread_server_main.cpp
  src/model/socket.cpp
  src/model/socket_data.cpp
  src/model/thread_safe_vector.cpp
  src/server/minimum_thread_server.cpp
  src/tools/logger.cpp
  )
target_compile_features(minimum_thread_server PRIVATE cxx_range_for)
