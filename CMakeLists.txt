cmake_minimum_required(VERSION 3.5)
project(VChat VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# debug and test
#set_target_properties(server PROPERTIES COMPILE_FLAGS "-g")

set(LIB ${CMAKE_SOURCE_DIR}/lib)
set(FTXUI_LIB ${LIB}/ftxui)
set(SQLITECPP_LIB ${LIB}/sqlitecpp)
set(GLOG_LIB ${LIB}/glog)
set(BOOST_LIB ${LIB}/boost)

find_package(ftxui REQUIRED PATHS ${FTXUI_LIB})
find_package(Boost REQUIRED PATHS ${BOOST_LIB})
find_package(SQLiteCpp REQUIRED PATHS ${SQLITECPP_LIB})
find_package(glog REQUIRED PATHS ${GLOG_LIB})

add_subdirectory(client)
add_subdirectory(server)
