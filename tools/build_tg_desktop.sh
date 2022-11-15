cd ../apps/TermGraphApp

echo "\n\n---CONAN INSTALL STARTED---\n" &&
conan install ./ -pr conanfiles/profile/desktop_debug -g=cmake_paths -g=json -g=cmake --build=missing -if=build/dev/conan-dependencies &&

echo "\n\n---CMAKE CONFIGURE STARTED---\n" &&
cmake --preset dev ./ &&

echo "\n\n---CMAKE BUILD STARTED---\n" &&
cmake --build --preset dev
