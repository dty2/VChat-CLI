#! /bin/bash

boost_version="boost_1_85_0"
boost_1_85_0="https://archives.boost.io/release/1.85.0/source/boost_1_85_0.tar.gz"
glog="git@github.com:google/glog.git"
sqlite3="git@github.com:SRombauts/SQLiteCpp.git"
json="git@github.com:nlohmann/json.git"
ftxui="git@github.com:ArthurSonzogni/FTXUI.git"
command=("wget" "git" "cmake" "make" "tar")

project_dir=$vchat
install_dir=$vchat/lib
target_dir=$vchat/libs

check_command() {
  echo "Check Command ..."
  for item in ${command[@]}; do
    if ! command -v $item >/dev/null 2>&1; then
      echo "$item not exist"
      echo "please get $item and try again"
      exit 0
    fi
  done
  echo "Check Pass"
}

install() {
  choice=
  echo "Install all libs"
  echo "Type y/n to continue"
  read choice
  if [ "$choice" = "y" ]; then
    mkdir "$install_dir"
    if [ "$1" ]; then
      cd "$install_dir" && wget --proxy=on "$boost_1_85_0" && tar -xzvf "$boost_version".tar.gz
    else
      cd "$install_dir" && wget --proxy=off "$boost_1_85_0" && tar -xzvf "$boost_version".tar.gz
    fi
    cd "$install_dir" && \
    git clone "$glog" && git clone "$sqlite3" && \
    git clone "$json" && git clone "$ftxui"
  else
    echo "Cancel install"
    exit 0
  fi
}

compile_item() {
  echo "Start compile $1"
  cd $target_dir && mkdir "$1"
  cd $install_dir/$1 && mkdir build && \
  cd build && cmake -DCMAKE_INSTALL_PREFIX=$target_dir/$1 .. && \
  make -j8 && make install
}

compile() {
  choice=
  echo "compile all libs"
  echo "Type y/n to continue"
  read choice
  if [ "$choice" = "y" ]; then
    mkdir $target_dir
    compile_item glog
    compile_item SQLiteCpp
    compile_item json
    compile_item FTXUI
    mkdir $target_dir/$boost_version
    cd $install_dir/$boost_version && ./bootstrap.sh && \
    ./b2 install --prefix=$target_dir/$boost_version
  else
    echo "Cancel compile"
    exit 0
  fi
}

main() {
  check_command
  install "$1"
  compile
  rm -rf "$install_dir"
}

main "$1"
