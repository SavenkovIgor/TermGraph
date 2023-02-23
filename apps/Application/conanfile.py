from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
import os


class TermGraphConan(ConanFile):

    generators = 'CMakeToolchain', 'CMakeDeps'
    settings = 'os', 'arch', 'compiler', 'build_type'

    requires = 'outcome/2.2.3'

    def is_wasm(self):
        return self.settings.os == 'Emscripten'

    def build_requirements(self):
        if not self.is_wasm():
            self.test_requires('gtest/1.10.0')

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
