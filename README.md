# libdivsufsort-dna

libdivsufsort-dna is a command-line wrapper for the [divsufsort](https://github.com/y-256/libdivsufsort) library.
Only builds the BWT

## Dependencies

- [libdivsufsort](https://github.com/y-256/libdivsufsort) 

Build the 64 bit Version. The following commands should clone and build divsufsort properly:

```bash
git clone https://github.com/y-256/libdivsufsort.git
cd libdivsufsort
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX="/usr/local" -DBUILD_DIVSUFSORT64=ON -DUSE_OPENMP=ON ..
make
sudo make install
```

## Build

```bash
mkdir -p build
cd build
cmake -DBUILD_DSS -DBUILD_LIBSAIS -DBUILD_LIBCUBWT ..
make
```

## Command-Line-Tool

```
Usage: ./dss
    -h                              shows help.
    -i                              input file path. Accepted file format is FASTA file (both one line per read and multiple lines per read)
    -o                              output file path for the BWT.

```
