A toy image processing program writen in pure C.

## Dependencies
 - (Windows only) MSYS2/MinGW64
 - (Linux only) GCC
 - GTK+3
 - OpenBLAS

## Build Guide

### Linux
```shell script
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j`nproc`
```

### Windows

Before build, edit X:\path\to\msys64\mingw64\usr\lib\pkgconfig\openblas.pc.
```pkgconfig
prefix=/mingw64
libdir=${prefix}/lib
includedir=${prefix}/include/OpenBLAS
...
```

After that, follow Linux build guide.
