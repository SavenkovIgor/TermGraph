rm -rf ./build &&
#conan install -if ./conan -pr ./conan/profiles/linux ./ &&
#conan install -if ./conan -pr ./conan/profiles/linux ./ &&
conan install -if ./conan -pr ./conan/profiles/linux --build=qt ./ &&
mkdir -p build &&
chmod 777 build &&
conan build -bf ./build -if ./conan ./
