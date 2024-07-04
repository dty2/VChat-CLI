# .cpp
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES)
list(APPEND SOURCES
  "${SRC_DIR}/main.cpp"
  "${SRC_DIR}/function.cpp"
  "${SRC_DIR}/net.cpp"
  "${SRC_DIR}/tool.cpp"
  "${SRC_DIR}/data/package.cpp"
  "${SRC_DIR}/data/info.cpp"
  "${SRC_DIR}/ui/tui.cpp"
  "${SRC_DIR}/ui/dashboard.cpp"
  "${SRC_DIR}/ui/chat.cpp"
  "${SRC_DIR}/ui/help.cpp"
  "${SRC_DIR}/ui/about.cpp"
)

# .out
add_executable(client "${SOURCES}")

# .hpp
target_include_directories(client PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/data"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/ui"
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
