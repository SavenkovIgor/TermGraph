cd ../apps/TermGraphApp

echo "\n\n---CONAN INSTALL STARTED---\n" &&
conan install . --profile conanfiles/profile/dev_desktop --build=missing -if=build/dev_desktop/conan-dependencies &&

echo "\n\n---CMAKE CONFIGURE STARTED---\n" &&
cmake --preset dev_desktop ./ &&

echo "\n\n---CMAKE BUILD STARTED---\n" &&
cmake --build --preset dev_desktop
