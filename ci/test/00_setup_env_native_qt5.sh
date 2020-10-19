#!/usr/bin/env bash
#
# Copyright (c) 2019-2020 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

export CONTAINER_NAME=ci_native_qt5
export DOCKER_NAME_TAG=ubuntu:18.04  # Check that bionic can compile our c++17 and run our functional tests in python3
export PACKAGES="python3-zmq qtbase5-dev qttools5-dev-tools libdbus-1-dev libharfbuzz-dev"
export DEP_OPTS="NO_QT=1 NO_UPNP=1 DEBUG=1 ALLOW_HOST_PACKAGES=1"
# BEGINCOMMENT
: '
export TEST_RUNNER_EXTRA="--previous-releases --coverage --extended --exclude feature_dbcrash"  # Run extended tests so that coverage does not fail, but exclude the very slow dbcrash
'
# ENDCOMMENT
export TEST_RUNNER_EXTRA="--coverage --exclude rpc_bind" # TODO.ZENY.YESPOWER # TEST DISABLED # Due to timeout...
export RUN_SECURITY_TESTS="true"
export RUN_UNIT_TESTS_SEQUENTIAL="true"
export RUN_UNIT_TESTS="false"
export GOAL="install"
# BEGINCOMMENT
: '
export PREVIOUS_RELEASES_TO_DOWNLOAD="v0.15.2 v0.16.3 v0.17.2 v0.18.1 v0.19.1"
'
# ENDCOMMENT
YESPOWER_PATH=$TRAVIS_BUILD_DIR/src/crypto/yespower-1.0.1
# YESPOWER_OPTION="-fPIE -Wall -O2 -fomit-frame-pointer" # TODO.ZENY.YESPOWER # OPTION DISABLED # Due to timeout...
# YESPOWER_CFLAGS="CFLAGS='-I$YESPOWER_PATH $YESPOWER_OPTION'" # TODO.ZENY.YESPOWER # Already there a CFLAGS
export BITCOIN_CONFIG="--enable-zmq --with-gui=qt5 --enable-glibc-back-compat --enable-reduce-exports --enable-c++17 --enable-debug CFLAGS=\"-I$YESPOWER_PATH -g0 -O2 -funsigned-char\" CXXFLAGS=\"-g0 -O2 -funsigned-char\" --with-boost-process"
