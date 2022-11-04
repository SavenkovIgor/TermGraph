conan install -if ../conanfiles -pr ../conanfiles/profiles/linux -pr:b=default --update --build=missing ../
conan build -if ../conanfiles ../
