source ~/emsdk/emsdk_env.sh

cd ../apps/Application

echo "\n\n---CONAN INSTALL STARTED---\n" &&
conan install . --profile conanfiles/profile/wasm_release --build=missing -if=build/wasm_release/conan-dependencies &&

echo "\n\n---CMAKE CONFIGURE STARTED---\n" &&
cmake --preset wasm_release ./ &&

echo "\n\n---CMAKE BUILD STARTED---\n" &&
cmake --build --preset wasm_release
