# gen-dataset
A command line tool to quickly generate a lot of files in a lot of directories. This tool creates an [M-ary tree](https://en.wikipedia.org/wiki/M-ary_tree)
shaped directory tree and randomly places any number of files of any size within this tree. The distribution of files per directory is roughly equal. If a size is provided, the files will be filled with zeros up to that size.
![Usage example](doc/example.gif)
## Installation
### Precompiled Static Binary (Recommendation)
* Download Msys64
  ```sh
  https://github.com/msys2/msys2-installer/releases/download/2025-08-30/msys2-x86_64-20250830.exe
  ```
* Install compiler & extensions
  ```sh
  launch Msys64
  pacman -S mingw-w64-ucrt-x86_64-gcc
  pacman -S mingw-w64-ucrt-x86_64-fmt
  pacman -S mingw-w64-ucrt-x86_64-make
  ```
### From Source
* Get Source and Install
  ```sh
  git clone https://github.com/3bdurahman/gen-dataset.git
  cd gen-dataset
  cd build
  mingw32-make
  ```

### Usage
```
usage:
  gen-dataset  -c [-b -d -s -S -t -y] [path]

flags:
  -b, --branches <int>              - number of subdirectories per directory
  -c, --count <int>                 - total number of files to create
  -d, --depth <int>                 - number of directory levels
  -s, --size <float[K..T][i]B>      - file size
  -S, --buff-size <float[K..T][i]B> - write buffer size (default=1M)
  -t, --threads <int>               - number of parallel file creation threads
  -y, --yes                         - don't prompt before creating files
```
#### Example
Generate 10 1GiB files in a single subdirectory named 'subdir':
```sh
gen-dataset -c 10 -s 1G subdir
```
Generate 10,000 1M files in 3905 directories:
```sh
gen-dataset -d 5 -b 5 -c 10000 -s 1M
```
Generate 1,000,000 empty files in 55986 directories with 16 threads writing the files:
```sh
gen-dataset -d 6 -b 6 -c 1000000 -t 16
```

