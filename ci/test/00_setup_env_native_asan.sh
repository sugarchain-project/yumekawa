#!/usr/bin/env bash
#
# Copyright (c) 2019-2020 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

export CONTAINER_NAME=ci_native_asan
export PACKAGES="clang llvm python3-zmq qtbase5-dev qttools5-dev-tools libevent-dev bsdmainutils libboost-system-dev libboost-filesystem-dev libboost-test-dev libboost-thread-dev libdb5.3++-dev libminiupnpc-dev libzmq3-dev libqrencode-dev libsqlite3-dev"
export DOCKER_NAME_TAG=ubuntu:20.04
export NO_DEPENDS=1
export GOAL="install"
YESPOWER_PATH=/tmp/cirrus-ci-build/src/crypto/yespower-1.0.1
# YESPOWER_OPTION="-fPIE -Wall -O2 -fomit-frame-pointer" # Sugarchain: Settings Part 1
YESPOWER_CFLAGS="CFLAGS='-I$YESPOWER_PATH'"
export BITCOIN_CONFIG="$YESPOWER_CFLAGS --enable-zmq --with-incompatible-bdb --with-gui=qt5 CPPFLAGS='-DARENA_DEBUG -DDEBUG_LOCKORDER' --with-sanitizers=address,integer,undefined CC=clang CXX=clang++ --with-boost-process"
