#!/bin/bash

apt install -y git vim gcc-12 g++-12 cmake ninja-build python3 python3-pip &&
apt install -y libgl1-mesa-dev pkg-config xorg-dev libx11-xcb-dev libxcb*-dev xkb-data libfontconfig1-dev libxkbcommon-x11-dev &&
pip3 install conan
