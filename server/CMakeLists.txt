# Copyright (c) 2024 Hunter 执着
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# .cpp
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES)
list(APPEND SOURCES
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/net/server.cpp
  ${SRC_DIR}/net/connection.cpp
  ${SRC_DIR}/service/service.cpp
  ${SRC_DIR}/data/package.cpp
  ${SRC_DIR}/data/store.cpp
)

# .out
add_executable(server ${SOURCES})

# .hpp
target_include_directories(server PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/net"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/data"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/service"
  "${BOOST_LIB}/include"
  "${SQLITECPP_LIB}/include"
)

# .lib
target_link_libraries(server
  ${BOOST_LIB}/lib/libboost_system.a
  ${BOOST_LIB}/lib/libboost_thread.a
  ${BOOST_LIB}/lib/libboost_serialization.a
  pthread
  jsoncpp
  SQLiteCpp
  absl::flags
  absl::flags_parse
  glog::glog
)
