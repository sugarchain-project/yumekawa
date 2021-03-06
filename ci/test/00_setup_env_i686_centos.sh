#!/usr/bin/env bash
#
# Copyright (c) 2020 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

export HOST=i686-pc-linux-gnu
export CONTAINER_NAME=ci_i686_centos_7
export DOCKER_NAME_TAG=centos:7
export DOCKER_PACKAGES="gcc-c++ glibc-devel.x86_64 libstdc++-devel.x86_64 glibc-devel.i686 libstdc++-devel.i686 ccache libtool make git python3 python36-zmq which patch lbzip2 dash"
export RUN_FUNCTIONAL_TESTS="false"
export GOAL="install"
YESPOWER_PATH=$TRAVIS_BUILD_DIR/src/crypto/yespower-1.0.1
# YESPOWER_OPTION="-fPIE -Wall -O2 -fomit-frame-pointer" # Sugarchain: Settings Part 1
YESPOWER_CFLAGS="CFLAGS='-I$YESPOWER_PATH'"
export BITCOIN_CONFIG="$YESPOWER_CFLAGS --enable-zmq --with-gui=qt5 --enable-reduce-exports --with-boost-process"
export CONFIG_SHELL="/bin/dash"
