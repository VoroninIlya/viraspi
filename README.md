### Prepare MSYS2 and compilers for ARM Cross-compaling

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
- Install other arm-gnu-toolchain in folder 'C:\\msys64\\mingw64' for needed host and target [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
- Install [mingw-w64-x86_64-cmake](https://packages.msys2.org/package/mingw-w64-x86_64-cmake?repo=mingw64)
```
pacman -S mingw-w64-x86_64-cmake
```
- Install ninja (should be automatically installed with mingw-w64-x86_64-cmake)
```
pacman -S mingw-w64-x86_64-ninja
```
- Add to the user PATH 'C:\\msys64\\mingw64\\bin'
