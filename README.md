![Start](/resource/example/start.png)
![Chat](/resource/example/chat.png)
[English](/README.md) [中文](/README_cn.md)  
# VChat
VChat is a TUI(Text User Interface) Chat Program, which supports communication under the terminal.  
## Start
### Install
You can download bin from release or build it yourself.  
### Build
I write a deploy.sh, so you can build it in five steps.  
* Step One, download a Nerd Font that you like to show interestring icons.  
  1. Download a .zip file from [Nerd Fonts](https://www.nerdfonts.com/font-downloads) and unzip it.  
  2. Move .ttf file to `$HOME/.local/share/fonts/`  
* Step Two, write `export vchat=$HOME/VChat` to your .bashrc or .zshrc  
* Step Three, create a directory and clone repository.  
  1. `mkdir $HOME/VChat`  
  2. `git clone git@github.com:dty2/vchat.git` or `git clone https://github.com/dty2/vchat.git`(if you don't have SSH)  
* Step Four, run `bash deploy.sh` to deploy VChat.  
* Step Five, Chat!  
  - `./server`  
  - `./client`  
## About
Latest version is 0.7.8  
Why is 0.7.8? There is a chinese saying that "完成的七七八八"(almost done)  
### Feature
* Full keyboard operation, no mouse needed  
* No graphics, can even run at tty  
* Many kinds of BUGS... hhh  
### Library
* [FTXUI](https://github.com/ArthurSonzogni/FTXUI)  
* [boost](https://www.boost.org/users/download/)  
* [sqlitecpp](https://github.com/SRombauts/SQLiteCpp)  
* [json](https://github.com/nlohmann/json)  
* [abseil](https://github.com/abseil/abseil-cpp)  
* [glog](https://github.com/google/glog)  
### Tools
* [Find icon font](https://www.nerdfonts.com/cheat-sheet)  
* [ascii font](https://docs.asciinema.org/)  
* Cowsay, Downlaod cowsay with `sudo pacman -S cowsay`  
## FAQ
* "deploy.sh" is not working  
I wrote a lot of comments in "deploy.sh". Check which step wrong and modify it yourself.  
Or you can ask me in issue.  
* VChat is not working  
You can write an issue, and I will help you as fast as possible.  
