#!/usr/bin/env bash
#
# Copyright (c) 2019-2020 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

export CONTAINER_NAME=ci_win64
export DOCKER_NAME_TAG=ubuntu:18.04  # Check that bionic can cross-compile to win64 (bionic is used in the gitian build as well)
export HOST=x86_64-w64-mingw32
export PACKAGES="python3 nsis g++-mingw-w64-x86-64 wine-binfmt wine64"
export RUN_FUNCTIONAL_TESTS=false
export RUN_SECURITY_TESTS="true"
export GOAL="deploy"
YESPOWER_PATH=$TRAVIS_BUILD_DIR/src/crypto/yespower-1.0.1
# YESPOWER_OPTION="-fPIE -Wall -O2 -fomit-frame-pointer" # TODO.ZENY.YESPOWER # OPTION DISABLED # Due to timeout...
YESPOWER_CFLAGS="CFLAGS='-I$YESPOWER_PATH'"
YESPOWER_WIN_OPTION="--disable-shared" # TODO.ZENY.YESPOWER # Build is failing without this flag...
export BITCOIN_CONFIG="$YESPOWER_CFLAGS $YESPOWER_WIN_OPTION --enable-reduce-exports --disable-gui-tests --without-boost-process"
