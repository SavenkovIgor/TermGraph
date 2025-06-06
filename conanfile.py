from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
import os


class TermGraphConan(ConanFile):

    generators = 'CMakeDeps'
    settings = 'os', 'arch', 'compiler', 'build_type'

    # requires = ''

    def is_wasm(self):
        return self.settings.os == 'Emscripten'

    def build_requirements(self):
        if not self.is_wasm():
            self.test_requires('gtest/1.15.0')

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False  # Disable user presets generation
        tc.generate()
