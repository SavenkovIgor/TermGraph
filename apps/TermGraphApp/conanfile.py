from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake


class TermGraphConan(ConanFile):

    generators = 'CMakeToolchain', 'CMakeDeps'
    settings = 'os', 'arch', 'compiler', 'build_type'

    requires = 'outcome/2.2.3'

    options = {'with_qt':           [True, False],
               'build_application': [True, False],
               'build_tests':       [True, False]}

    default_options = {'with_qt':           False,
                       'build_application': True,
                       'build_tests':       True}

    def build_requirements(self):
        if self.options.build_tests:
            self.test_requires('gtest/1.10.0')

    def requirements(self):
        if self.options.with_qt:
            self.requires('qtbase/6.3.1@qt/everywhere')

            if self.options.build_application:
                self.requires('qtdeclarative/6.3.1@qt/everywhere')
                self.requires('qtimageformats/6.3.1@qt/everywhere')
                self.requires('qtshadertools/6.3.1@qt/everywhere')
                self.requires('qtsvg/6.3.1@qt/everywhere')

    def generate(self):
        tc = CMakeToolchain(self)

        if self.options.with_qt:
            tc.cache_variables["BUILD_APPLICATION"] = self.options.build_application
            tc.cache_variables["BUILD_TESTING"] = self.options.build_tests

        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
