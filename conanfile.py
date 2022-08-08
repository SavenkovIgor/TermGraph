from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake


class TermGraphConan(ConanFile):

    generators = 'CMakeToolchain', 'CMakeDeps'
    settings = 'os', 'arch', 'compiler', 'build_type'

    requires = 'outcome/2.2.3'

    options = {'with_qt':           [True, False],
               'build_application': [True, False],
               'build_backend':     [True, False],
               'build_frontend':    [True, False],
               'build_tests':       [True, False]}

    default_options = {'with_qt':             False,
                       'build_application':   True,
                       'build_backend':       True,
                       'build_frontend':      False,
                       'build_tests':         False}

    def requirements(self):
        if self.options.with_qt:
            self.requires('qtbase/6.3.1@qt/everywhere')
            self.requires('qtdeclarative/6.3.1@qt/everywhere')
            self.requires('qtimageformats/6.3.1@qt/everywhere')
            self.requires('qtshadertools/6.3.1@qt/everywhere')
            self.requires('qtsvg/6.3.1@qt/everywhere')

        if self.options.build_backend:
            self.requires('restinio/0.6.14')

        if self.options.build_application and self.options.build_tests:
            self.requires('gtest/1.10.0')

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
