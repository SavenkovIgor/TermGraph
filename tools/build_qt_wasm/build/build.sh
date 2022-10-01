# Exec manually!
source "/home/user/emsdk/emsdk_env.sh"

# Qt root path
QT_ROOT=$HOME/Qt
QT_VERSION=6.4.0

# cmake path
PATH=$PATH:$QT_ROOT/Tools/CMake/bin
# ninja path
PATH=$PATH:$QT_ROOT/Tools/Ninja

# CMAKE_TOOLCHAIN_FILE=/home/user/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake

# CONFIG_PLATFORM=linux-g++
CONFIG_PLATFORM=wasm-emscripten

CONFIG_HOST_PATH=$QT_ROOT/$QT_VERSION/gcc_64
# CONFIG_HOST_PATH=$QT_ROOT/$QT_VERSION/wasm_32

CONFIG_MODULES='qtbase,qtsvg,qtimageformats,qtshadertools,qtdeclarative'

echo "---Cloning---"
git clone git://code.qt.io/qt/qt5.git ../source
cd ../source

echo "---Checkout---"
git checkout v$QT_VERSION

echo "---Init-repository---"
perl init-repository # --module-subset=$CONFIG_MODULES

cd ../build

# THREAD_ARG=
THREAD_ARG='-feature-thread'

echo "---Configure---"
../source/configure -qt-host-path $CONFIG_HOST_PATH -platform $CONFIG_PLATFORM $THREAD_ARG -prefix $QT_ROOT/$QT_VERSION/wasm_32_mt -submodules $CONFIG_MODULES

echo "---Build---"
cmake --build . --parallel 4
cmake --install .
