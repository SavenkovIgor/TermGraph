cd ../apps/TermGraphApp

echo "\n\n---CONAN INSTALL STARTED---\n" &&
conan install . --profile conanfiles/profile/desktop_dev --build=missing -if=build/desktop_dev/conan-dependencies &&

echo "\n\n---CMAKE CONFIGURE STARTED---\n" &&
cmake --preset desktop_dev ./ &&

echo "\n\n---CMAKE BUILD STARTED---\n" &&
cmake --build --preset desktop_dev
