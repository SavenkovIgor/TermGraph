cd ../apps/Application

echo "\n\n---CONAN INSTALL STARTED---\n" &&
conan install . --profile conanfiles/profile/desktop_release --build=missing -if=build/desktop_release/conan-dependencies &&

echo "\n\n---CMAKE CONFIGURE STARTED---\n" &&
cmake --preset desktop_release ./ &&

echo "\n\n---CMAKE BUILD STARTED---\n" &&
cmake --build --preset desktop_release
