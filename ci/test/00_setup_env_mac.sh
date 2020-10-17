#!/usr/bin/env bash
#
# Copyright (c) 2019-2020 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

export CONTAINER_NAME=ci_macos_cross
export HOST=x86_64-apple-darwin16
export PACKAGES="cmake imagemagick libcap-dev librsvg2-bin libz-dev libbz2-dev libtiff-tools python3-dev python3-setuptools"
export XCODE_VERSION=11.3.1
export XCODE_BUILD_ID=11C505
export RUN_UNIT_TESTS=false
export RUN_FUNCTIONAL_TESTS=false
export GOAL="deploy"
YESPOWER_PATH=$TRAVIS_BUILD_DIR/src/crypto/yespower-1.0.1
YESPOWER_OPTION="-fPIE -Wall -O2 -fomit-frame-pointer"
export BITCOIN_CONFIG="CFLAGS='-I$YESPOWER_PATH $YESPOWER_OPTION' --with-gui --enable-reduce-exports --enable-werror --with-boost-process"
