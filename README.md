# St_Nicholas
Dropping Gifts


### build with make

For make and build project:

```sh
  make all [BINPATH="/path/to/arm-none-eabi-gcc"] -j 12
```
  
example:

```sh
  make all BINPATH=C:/ARM_GCC/10_2021.10/bin -j 12
```


### build with cmake

For build with cmake you should have proper compiler in PATH env variable.

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build build --target all
```


