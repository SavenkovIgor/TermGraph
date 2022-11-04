conan install -if ../conanfiles -pr ../conanfiles/profiles/linux -pr:b=default -o with_qt=True --update --build=missing ../
conan build -if ../conanfiles ../
