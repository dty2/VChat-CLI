# .cpp
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES)
list(APPEND SOURCES
  "${SRC_DIR}/main.cpp"
  "${SRC_DIR}/control.cpp"
  "${SRC_DIR}/transmission.cpp"
  "${SRC_DIR}/tui.cpp"
)

# .out
add_executable(client "${SOURCES}")

# .hpp
target_include_directories(client
  PUBLIC 
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${Ftxui_INCLUDE_DIRS}"
  "${SQLiteCpp_INCLUDE_DIRS}"
)

# .lib
target_link_libraries(client 
  ftxui::screen
  ftxui::dom
  ftxui::component
  SQLiteCpp
  glog::glog
)


