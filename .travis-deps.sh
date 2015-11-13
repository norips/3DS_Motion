#!/bin/sh
set -e
set -x

# Build and install devkitARM + ctrulib
wget http://sourceforge.net/projects/devkitpro/files/Automated%20Installer/devkitARMupdate.pl
git clone https://github.com/smealum/ctrulib.git
sudo perl devkitARMupdate.pl

# Get latest ctrulib and overwrite bundled one
cd ctrulib/libctru && make ; cd -
sudo cp -rf ctrulib/libctru/ ${DEVKITPRO}

# Get giflib and install
mkdir portlibs && cd portlibs
git clone https://github.com/norips/3ds_portlibs.git
wget -O giflib-5.1.1.tar.bz2 http://sourceforge.net/projects/giflib/files/giflib-5.1.1.tar.bz2/download
make giflib && make install
