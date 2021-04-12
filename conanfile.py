from conans import ConanFile
# from conans import CMake

# Please add to default profile for success compilation of TermGraph
# conan profile update settings.compiler.libcxx=libstdc++11 default

# WARNING: This file is used only for gtest delivery


class TermGraphConan(ConanFile):

    requires = "gtest/1.10.0"

    generators = "cmake_find_package", "cmake_paths"

#    def build(self):
#        cmake = CMake(self)
#        cmake.configure()
#        cmake.build()
