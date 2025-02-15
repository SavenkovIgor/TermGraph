from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
import os


class TermGraphConan(ConanFile):

    generators = 'CMakeToolchain', 'CMakeDeps'
    settings = 'os', 'arch', 'compiler', 'build_type'

    # requires = ''

    def is_wasm(self):
        isEmscripten = self.settings.os == 'Emscripten'
        isClang = self.settings.compiler == 'clang'
        return isEmscripten or isClang

    def build_requirements(self):
        if not self.is_wasm():
            self.test_requires('gtest/1.15.0')

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
