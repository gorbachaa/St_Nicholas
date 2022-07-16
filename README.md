# St_Nicholas
Dropping Gifts


### build with make

For build with make you should have proper compiler in PATH env variable.

```sh
  make clean
  make all -j 12
```


### build with cmake

For build with cmake you should have proper compiler in PATH env variable.

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build build --target all
```


