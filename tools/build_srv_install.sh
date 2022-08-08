#!/bin/bash

test -f .build.env &&

echo "\n\n\nUpdating" &&
apt update            &&

echo "\n\n\nInstall basic tools"                                           &&
apt install -y git vim gcc-12 g++-12 cmake ninja-build python3 python3-pip &&

echo "\n\n\nInstall additional qt build libraries"                                                                                       &&
apt install -y libgl-dev libgl1-mesa-dev pkg-config xorg-dev libx11-xcb-dev libxcb*-dev xkb-data libfontconfig1-dev libxkbcommon-x11-dev &&

echo "\n\n\nConan install" &&
pip3 install conan         &&

echo "\n\n\nAdd qt remote"                                              &&
conan remote add qt https://qtpkgtest.jfrog.io/artifactory/api/conan/qt &&

echo "\n\n\nConan configure"                   &&
conan config set general.revisions_enabled=1   &&
conan config set general.cmake_generator=Ninja &&

echo "\n\n\nSet env variables"
# Export env vars
set -o allexport
source .build.env
set +o allexport

