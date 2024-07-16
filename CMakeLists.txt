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

cmake_minimum_required(VERSION 3.5)
project(VChat VERSION 0.5.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set project status
SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_BUILD_TYPE "Release") # uncomment to release

# generate compile_commands.json for clangd
# which give you some auto-completion information when you coding...
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIB ${CMAKE_SOURCE_DIR}/libs)
set(FTXUI_LIB ${LIB}/FTXUI)
set(BOOST_LIB ${LIB}/boost_1_85_0)
set(JSONCPP_LIB ${LIB}/json)
set(SQLITECPP_LIB ${LIB}/SQLiteCpp)
set(GLOG_LIB ${LIB}/glog)
set(ABSEIL_LIB ${LIB}/abseil)

find_package(ftxui REQUIRED PATHS ${FTXUI_LIB})
find_package(Boost 1.85.0 REQUIRED COMPONENTS system thread serialization PATHS ${BOOST_LIB})
find_package(nlohmann_json REQUIRED PATHS ${JSONCPP_LIB})
find_package(SQLiteCpp REQUIRED PATHS ${SQLITECPP_LIB})
find_package(glog REQUIRED PATHS ${GLOG_LIB})
find_package(absl REQUIRED PATHS ${ABSEIL_LIB})

add_subdirectory(client)
add_subdirectory(server)

# "-g" to compile can use gdb to debug
set_target_properties(client PROPERTIES COMPILE_FLAGS "-g")
set_target_properties(server PROPERTIES COMPILE_FLAGS "-g")
