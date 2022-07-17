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
                       'build_tests':         False,
                       'qt:qtdeclarative':    True,
                       'qt:qtquickcontrols2': True,
                       'qt:qtsvg':            True,
                       'qt:qtimageformats':   True}

    def requirements(self):
        if self.options.with_qt:
            self.requires('qt/6.2.1')

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
