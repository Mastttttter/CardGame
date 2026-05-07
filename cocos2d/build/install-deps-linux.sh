#!/usr/bin/env bash
set -euo pipefail

echo "This shell script will install dependencies for cocos2d-x on Arch Linux."
read -r -p "Continue? (y/n) " answer
case "$answer" in
    [Yy]*)
        echo "It will take a few minutes."
        ;;
    *)
        exit 0
        ;;
esac

if ! command -v pacman >/dev/null 2>&1; then
    echo "Not an Arch/pacman system. Please install dependencies manually."
    exit 0
fi

DEPENDS=(
    libx11
    libxmu
    glu
    gl2ps
    libxi
    libzip
    libpng
    curl
    fontconfig
    sqlite
    glew
    openssl
    gtk3
    binutils
    cmake
    gcc
    make
)

if command -v pacman-conf >/dev/null 2>&1; then
    repos="$(pacman-conf --repo-list)"
    if [[ $'\n'"$repos"$'\n' == *$'\nmultilib\n'* ]]; then
        DEPENDS+=(
            lib32-glibc
            lib32-ncurses
            lib32-gcc-libs
        )
    fi
fi

MISSING=()
for package in "${DEPENDS[@]}"; do
    if ! pacman -Qi "$package" >/dev/null 2>&1; then
        MISSING+=("$package")
    fi
done

if ((${#MISSING[@]})); then
    echo "Missing packages: ${MISSING[*]}"
    echo "You may be asked for your password for package installation."
    sudo pacman -Syu --needed "${DEPENDS[@]}"
else
    echo "All required packages are already installed."
fi

echo "Cocos uses GCC Version:"
gcc --version
echo "Cocos uses G++ Version:"
g++ --version
echo "Cocos uses ld Version:"
ld --version
