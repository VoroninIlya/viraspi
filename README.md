### Prepare MSYS2 and compilers for ARM Cross-compaling

#vscode #cross-compiling #msys2 #mingw64

- Install [MSYS2](https://github.com/msys2/msys2-installer/releases/download/2024-01-13/msys2-x86_64-20240113.exe)
- Update packages
```
pacman -Suy
```
- Install [mingw-w64-x86_64-toolchain](https://packages.msys2.org/groups/mingw-w64-x86_64-toolchain)
```
pacman -S mingw-w64-x86_64-toolchain
```
- Install [mingw-w64-x86_64-arm-none-eabi-toolchain](https://packages.msys2.org/groups/mingw-w64-x86_64-arm-none-eabi-toolchain)
```
pacman -S mingw-w64-x86_64-arm-none-eabi-toolchain
```
- Install other arm-gnu-toolchain in folder 'C:\\msys64\\mingw64' for needed host and target [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) (!NOTE: on my raspberry is glib version 2.31 installed, so I have loaded GNU Toolchain gcc-arm-10.2-2020.11 for the Arm architecture so that gclib matching, but after that i have replaced aarch64-none-linux-gnu-gdb-10.2.1 and aarch64-none-linux-gnu-gdb-add-index-10.2.1 with files from version 13.3)
- To check what gclib version is installed on raspi
```
ldd --version
```
- Install [mingw-w64-x86_64-cmake](https://packages.msys2.org/package/mingw-w64-x86_64-cmake?repo=mingw64)
```
pacman -S mingw-w64-x86_64-cmake
```
- Install ninja
```
pacman -S ninja
```
- Add to the user PATH 'C:\\msys64\\mingw64\\bin' and move it up in the liste
- Install open ssh
```
pacman -S openssh
```

### Installing and starting a gdb server on raspbrry pi
```
sudo apt update
sudo apt upgrade
sudo apt-get install gdbserver
gdbserver --multi :5555
```

