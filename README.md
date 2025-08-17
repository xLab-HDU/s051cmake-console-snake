# 双字节版的贪吃蛇
这个存储库是一个用 C++ 实现的双字节版的贪吃蛇游戏，用 CMake 构建。 

[![Build Status](https://github.com/xLab-HDU/s051cmake-console-snake/actions/workflows/ci.yml/badge.svg)](https://github.com/xLab-HDU/s051cmake-console-snake/actions)

## How to Use

```sh
# configure the project
cmake -B build

# build the project
cmake --build build

# test the project
cd build
ctest --output-on-failure

# run the project
./build/bin/main.exe

# package the project
cpack -G ZIP
# or
cpack -G NSIS
```

## License

The source code is dual licensed under Public Domain and MIT -- choose whichever you prefer.
