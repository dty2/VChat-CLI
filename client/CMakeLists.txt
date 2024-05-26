# .cpp
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES)
list(APPEND SOURCES
  "${SRC_DIR}/main.cpp"
  "${SRC_DIR}/function.cpp"
  "${SRC_DIR}/connection.cpp"
  "${SRC_DIR}/package.cpp"
  "${SRC_DIR}/tool.cpp"
  "${SRC_DIR}/info.cpp"
  "${SRC_DIR}/start.cpp"
)

# .out
add_executable(client "${SOURCES}")

# .hpp
target_include_directories(client PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${Ftxui_INCLUDE_DIRS}"
  "${BOOST_LIB}/include"
)

# .lib
target_link_libraries(client 
  ${BOOST_LIB}/lib/libboost_system.a
  ${BOOST_LIB}/lib/libboost_thread.a
  ${BOOST_LIB}/lib/libboost_serialization.a
  pthread
  ftxui::screen
  ftxui::dom
  ftxui::component
  jsoncpp
  glog::glog
)
