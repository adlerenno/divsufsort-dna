# libdivsufsort-dna

libdivsufsort-dna is a command-line wrapper for the [divsufsort](https://github.com/y-256/libdivsufsort) library.
Only builds the BWT

## Dependencies

- [libdivsufsort](https://github.com/y-256/libdivsufsort) 
- [libsais](https://github.com/IlyaGrebnov/libsais)

Build the 64 bit Version of divsufsort. The following commands should clone and build divsufsort properly:

```bash
git clone https://github.com/y-256/libdivsufsort.git
cd libdivsufsort
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX="/usr/local" -DBUILD_DIVSUFSORT64=ON -DUSE_OPENMP=ON ..
make
sudo make install
```

```bash
git clone https://github.com/IlyaGrebnov/libsais.git
cd libsais
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX="/usr/local" -DLIBSAIS_USE_OPENMP=ON -DLIBSAIS_BUILD_SHARED_LIB=ON ..
make
```

## Build

```bash
mkdir -p build
cd build
cmake -DBUILD_DSS=ON -DBUILD_LIBSAIS=ON -DBUILD_LIBCUBWT=ON ..
make
```

## Command-Line-Tool

```
Usage: ./dss
    -h                              shows help.
    -i                              input file path. Accepted file format is FASTA file (both one line per read and multiple lines per read)
    -o                              output file path for the BWT.
    -t                              number of threads. Only for sais-cli. Default is 1.

```
