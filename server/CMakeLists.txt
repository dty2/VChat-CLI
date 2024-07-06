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
  glog::glog
)
