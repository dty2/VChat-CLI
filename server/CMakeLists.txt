# .cpp
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES)
list(APPEND SOURCES
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/acceptor.cpp
  ${SRC_DIR}/service.cpp
  ${SRC_DIR}/store.cpp
  ${SRC_DIR}/package.cpp
  ${SRC_DIR}/connection.cpp
  ${SRC_DIR}/work.cpp
)

# .out
add_executable(server ${SOURCES})

# .hpp
target_include_directories(server PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${BOOST_LIB}/include
  ${SQLITECPP_LIB}/include
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
