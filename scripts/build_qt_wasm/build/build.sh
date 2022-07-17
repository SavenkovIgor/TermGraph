source "/home/user/emsdk/emsdk_env.sh"

# cmake path
PATH=$PATH:$HOME/Qt/Tools/CMake/bin
# ninja path
PATH=$PATH:$HOME/Qt/Tools/Ninja

# CMAKE_TOOLCHAIN_FILE=/home/user/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake

# CONFIG_PLATFORM=linux-g++
CONFIG_PLATFORM=wasm-emscripten

# CONFIG_HOST_PATH=/home/user/Qt/6.4.0/gcc_64
# CONFIG_HOST_PATH=/home/user/Qt/6.4.0/wasm_32
CONFIG_HOST_PATH=/home/user/Qt/6.3.1/gcc_64
# CONFIG_HOST_PATH=/home/user/Qt/6.3.1/wasm_32

CONFIG_MODULES='qtbase,qtsvg,qtimageformats,qtshadertools,qtdeclarative'

echo "---Cloning---"
git clone git://code.qt.io/qt/qt5.git ../source
cd ../source

echo "---Checkout---"
git checkout v6.3.1

echo "---Init-repository---"
perl init-repository # --module-subset=$CONFIG_MODULES

cd ../build

# THREAD_ARG=
THREAD_ARG='-feature-thread'

echo "---Configure---"
../source/configure -qt-host-path $CONFIG_HOST_PATH -platform $CONFIG_PLATFORM $THREAD_ARG -prefix /home/user/Qt/6.3.1/wasm_mt_build -submodules $CONFIG_MODULES

echo "---Build---"
cmake --build . --parallel 4
cmake --install .
