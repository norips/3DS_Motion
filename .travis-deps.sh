#!/bin/sh
set -e
set -x


# Get latest ctrulib and overwrite bundled one
git clone https://github.com/smealum/ctrulib.git
cd ctrulib/libctru && make ; cd -
cp -rf ctrulib/libctru/ ${DEVKITPRO}

# Get giflib and install
git clone https://github.com/norips/3ds_portlibs.git
cd 3ds_portlibs
wget -O giflib-5.1.1.tar.bz2 http://sourceforge.net/projects/giflib/files/giflib-5.1.1.tar.bz2/download
make giflib && make install
