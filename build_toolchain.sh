#!/usr/bin/env bash
set -euo pipefail

########################################
# Configuration
########################################

TARGET=i686-elf
PREFIX="$HOME/opt/cross"
SRCDIR="$HOME/src/cross"

BINUTILS_VER=2.45
GCC_VER=15.3.0

JOBS=$(nproc)

########################################
# Dependencies
########################################

echo "==> Checking dependencies..."

sudo pacman -Sy --needed \
    base-devel \
    gmp \
    mpfr \
    libmpc \
    isl \
    texinfo \
    wget \
    xz

########################################
# Directories
########################################

mkdir -p "$PREFIX"
mkdir -p "$SRCDIR"

export PREFIX
export TARGET
export PATH="$PREFIX/bin:$PATH"

cd "$SRCDIR"

########################################
# Download Binutils
########################################

if [ ! -d "binutils-${BINUTILS_VER}" ]; then
    echo "==> Downloading Binutils..."

    wget -nc \
    https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.xz

    tar xf binutils-${BINUTILS_VER}.tar.xz
fi

########################################
# Build Binutils
########################################

if [ ! -f "$PREFIX/bin/${TARGET}-ld" ]; then

    echo "==> Building Binutils..."

    rm -rf build-binutils

    mkdir build-binutils

    cd build-binutils

    ../binutils-${BINUTILS_VER}/configure \
        --target=$TARGET \
        --prefix=$PREFIX \
        --with-sysroot \
        --disable-nls \
        --disable-werror

    make -j$JOBS

    make install

    cd ..
else
    echo "==> Binutils already installed."
fi

########################################
# Download GCC
########################################

if [ ! -d "gcc-${GCC_VER}" ]; then

    echo "==> Downloading GCC..."

    wget -nc \
    https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.xz

    tar xf gcc-${GCC_VER}.tar.xz

fi

########################################
# Download GCC prerequisites
########################################

cd gcc-${GCC_VER}

./contrib/download_prerequisites

cd ..

########################################
# Build GCC
########################################

if [ ! -f "$PREFIX/bin/${TARGET}-gcc" ]; then

    echo "==> Building GCC..."

    rm -rf build-gcc
    mkdir build-gcc
    cd build-gcc

    # Inject CXXFLAGS here to prevent the C++20 libcody error on Arch
    CXXFLAGS="-fno-char8_t" ../gcc-${GCC_VER}/configure \
        --target=$TARGET \
        --prefix=$PREFIX \
        --disable-nls \
        --enable-languages=c \
        --without-headers

    make all-gcc -j$JOBS
    make all-target-libgcc -j$JOBS
    make install-gcc
    make install-target-libgcc

    cd ..
else
    echo "==> GCC already installed."
fi

########################################
# Add PATH if necessary
########################################

if ! grep -q "opt/cross/bin" "$HOME/.bashrc" 2>/dev/null; then

cat <<EOF >> "$HOME/.bashrc"

# Cross compiler
export PATH="\$HOME/opt/cross/bin:\$PATH"

EOF

fi

########################################
# Done
########################################

echo
echo "==========================================="
echo "Installed successfully!"
echo
echo "Compiler:"
echo "    $PREFIX/bin/${TARGET}-gcc"
echo
echo "Assembler:"
echo "    $PREFIX/bin/${TARGET}-as"
echo
echo "Linker:"
echo "    $PREFIX/bin/${TARGET}-ld"
echo
echo "Run:"
echo
echo "    source ~/.bashrc"
echo
echo "Then verify:"
echo
echo "    ${TARGET}-gcc --version"
echo "==========================================="
