![Start](/resource/example/start.png)
![Chat](/resource/example/chat.png)
[中文](/README_cn.md) [English](/README.md)  
# VChat
VChat 是一个TUI()聊天程序，支持在终端下通讯  
## 开始
### 安装
你可以从发行中下载二进制程序或者自己构建  
### Build
我写了一个部署脚本，所以你可以通过五步来构建它  
* 第一步，安装一个你喜欢的Nerd Font字体用来显示有趣的图标  
  1. 从[Nerd Fonts](https://www.nerdfonts.com/font-downloads)下载一个.zip文件并且解压它  
  2. 移动.ttf文件到目录`$HOME/.local/share/fonts/`  
* 第二步，向你的.bashrc或.zshrc中写入`export vchat=$HOME/VChat`  
* 第三步，创建目录并克隆仓库  
  1. `mkdir $HOME/VChat`  
  2. `git clone git@github.com:dty2/vchat.git` 或 `git clone https://github.com/dty2/vchat.git`(如果你没有SSH)  
* 第四步，执行`bash deploy.sh`部署VChat  
* 第五步，~~原神，启动！~~ 开始聊天！  
  - `./server`  
  - `./client`  
## 关于
最新版本 0.7.8  
为什么是0.7.8？因为搞的七七八八罢了  
### 特点
* 全键盘操作，不用鼠标  
* 没有图形界面，甚至能在跑在TTY上  
* 许多BUG... hhh  
### 库
* [FTXUI](https://github.com/ArthurSonzogni/FTXUI)  
* [boost](https://www.boost.org/users/download/)  
* [sqlitecpp](https://github.com/SRombauts/SQLiteCpp)  
* [json](https://github.com/nlohmann/json)  
* [abseil](https://github.com/abseil/abseil-cpp)  
* [glog](https://github.com/google/glog)  
### 工具
* [搜索图形字体网站](https://www.nerdfonts.com/cheat-sheet)  
* [生成Ascll艺术字](https://docs.asciinema.org/)  
* 牛说，通过`sudo pacman -S cowsay`安装  
## FAQ
* 部署脚本出错  
我在deploy.sh中写了很多注释。你可以自己逐步排查问题并且修改它  
或者你也可以在issue上问我  
* VChat出错  
你可以提一个issue，我会尽快回复并且帮助你  
