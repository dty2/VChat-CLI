# .cpp
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES)
list(APPEND SOURCES
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/transmission.cpp
  ${SRC_DIR}/service.cpp
  ${SRC_DIR}/store.cpp
)

# .out
add_executable(server "${SOURCES}")

# .hpp
target_include_directories(server PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${BOOST_LIB}/include
  ${SQLITECPP_LIB}/include
)

# .lib
target_link_libraries(server
  boost_system
  boost_thread
  boost_serialization
  pthread
  SQLiteCpp
  glog::glog
)
