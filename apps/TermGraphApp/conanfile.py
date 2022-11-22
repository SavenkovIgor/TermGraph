from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
import os


class TermGraphConan(ConanFile):

    generators = 'CMakeToolchain', 'CMakeDeps'
    settings = 'os', 'arch', 'compiler', 'build_type'

    requires = 'outcome/2.2.3'

    options = {'build_tests': [True, False]}
    default_options = {'build_tests': True}

    def is_wasm(self):
        toolchain_path = os.environ['CONAN_CMAKE_TOOLCHAIN_FILE']
        return 'wasm' in toolchain_path.lower()

    def build_requirements(self):
        if self.options.build_tests and not self.is_wasm():
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
