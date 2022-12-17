#!/bin/bash

variable_exist() {
  local varname=$1
  local example_msg=$2
if [[ -z "${!varname}" ]]; then
    printf "Error: variable not defined $varname\nCan be defined as: $example_msg\n"
    exit 1
fi
}

folder_exist() {
  local folder_path_var=$1
  if [[ ! -d "${!folder_path_var}" ]]; then
    printf "Error: folder not exist at ${folder_path_var} (${!folder_path_var})\n"
    exit 1
  else
    printf "Folder exist at ${folder_path_var} (${!folder_path_var})\n"
  fi
}

file_exist() {
  local file_path_var=$1
  if [[ ! -f "${!file_path_var}" ]]; then
    printf "Error: file not exist at ${file_path_var} (${!file_path_var})\n"
    exit 1
  else
    printf "File exist at ${file_path_var} (${!file_path_var})\n"
  fi
}

variable_exist QT_ROOT    "~/Qt"
variable_exist QT_VERSION "6.4.1"

# If fails here, exec emscripten evn script manually!
# source "$HOME/emsdk/emsdk_env.sh"
variable_exist EMSDK      "~/emsdk"
variable_exist EM_CONFIG  "~/emsdk/.emscripten"
variable_exist EMSDK_NODE "~/emsdk/node/14.18.2_64bit/bin/node"

QT_VERSION_PATH=$QT_ROOT/$QT_VERSION

folder_exist QT_ROOT
folder_exist QT_VERSION_PATH
folder_exist EMSDK
file_exist EM_CONFIG
file_exist EMSDK_NODE

CONFIG_PLATFORM=wasm-emscripten
CONFIG_HOST_PATH=$QT_VERSION_PATH/gcc_64
CONFIG_MODULES='qtbase,qtsvg,qtimageformats,qtshadertools,qtdeclarative'

echo "---Cloning---"
cd ..
git clone git://code.qt.io/qt/qt5.git ./source
cd ./source

echo "---Checkout---"
git checkout v$QT_VERSION

echo "---Init-repository---"
perl init-repository

THREAD_ARG='-feature-thread'

echo "---Configure---"
rm -rf qtwebengine
./configure -qt-host-path $CONFIG_HOST_PATH -platform $CONFIG_PLATFORM $THREAD_ARG -prefix $QT_VERSION_PATH/wasm_32_mt -submodules $CONFIG_MODULES -skip 'qtwebengine'

echo "---Build---"
cmake --build . --parallel 4
cmake --install .
