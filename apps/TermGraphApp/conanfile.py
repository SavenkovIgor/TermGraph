from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake


class TermGraphConan(ConanFile):

    generators = 'CMakeToolchain', 'CMakeDeps'
    settings = 'os', 'arch', 'compiler', 'build_type'

    requires = 'outcome/2.2.3'

    options = {'build_tests': [True, False]}
    default_options = {'build_tests': True}

    def build_requirements(self):
        if self.options.build_tests:
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
