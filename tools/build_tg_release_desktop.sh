cd ../apps/TermGraphApp

echo "\n\n---CONAN INSTALL STARTED---\n" &&
conan install . --profile conanfiles/profile/release_desktop --build=missing -if=build/release_desktop/conan-dependencies &&

echo "\n\n---CMAKE CONFIGURE STARTED---\n" &&
cmake --preset release_desktop ./ &&

echo "\n\n---CMAKE BUILD STARTED---\n" &&
cmake --build --preset release_desktop
