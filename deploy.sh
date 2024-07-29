#! /bin/bash

# The below content is third-party library download address
boost_version="boost_1_85_0"
boost_1_85_0="https://archives.boost.io/release/1.85.0/source/boost_1_85_0.tar.gz"
glog="git@github.com:google/glog.git"
sqlite3="git@github.com:SRombauts/SQLiteCpp.git"
json="git@github.com:nlohmann/json.git"
ftxui="git@github.com:ArthurSonzogni/FTXUI.git"
absl="git@github.com:abseil/abseil-cpp.git"

# directory
## you need add vchat environment variable to specify the download directory
project_dir=$vchat
install_dir=$project_dir/lib
target_dir=$project_dir/libs

# check install environment
# the commands which is needed to use
commands=("wget" "git" "cmake" "make" "tar")
check_env() {
  echo "check command..."
  for item in ${commands[@]}; do
    if ! command -v $item >/dev/null 2>&1; then
      echo "$item not exist"
      echo "please get $item commnad and try again"
      exit 0
    fi
  done
  echo "check pass"
  echo "check environment..."
  if [ -z $vchat ]; then
    echo "vchat environment not exist"
    echo "please add 'vchat' environment variable"
  fi
  echo "check pass"
}

# install all library
install() {
  choice=
  echo "Install all libs"
  echo "Type y/n to continue"
  read choice
  if [ "$choice" = "y" ]; then
    mkdir "$install_dir"
    cd "$install_dir" && wget --proxy=on "$boost_1_85_0" && tar -xzvf "$boost_version".tar.gz
    cd "$install_dir" && \
    git clone "$glog" && git clone "$sqlite3" && \
    git clone "$json" && git clone "$ftxui"
  else
    echo "Cancel install"
    exit 0
  fi
}

# compile each library
compile_item() {
  echo "Start compile $1"
  cd $target_dir && mkdir "$1"
  cd $install_dir/$1 && mkdir build && \
  cd build && cmake -DCMAKE_INSTALL_PREFIX=$target_dir/$1 .. && \
  make -j8 && make install
  echo "Compile $1 finish"
}

# compile all library
compile() {
  choice=
  echo "Compile all third-party libraries"
  echo "Type y/n to continue"
  read choice
  if [ "$choice" = "y" ]; then
    mkdir $target_dir
    compile_item glog
    compile_item absl
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
  check_env
  install
  compile
  choice=
  echo "Compile finish, do you want to remove all install directory"
  echo "Type y/n to continue"
  read choice
  if [ "$choice" = "y" ]; then
    rm -rf "$install_dir"
  fi
}

main
