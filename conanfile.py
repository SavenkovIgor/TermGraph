from conans import ConanFile, CMake


class TermGraphConan(ConanFile):

    generators = 'cmake_find_package', 'cmake_paths'

    requires = 'outcome/2.2.1'

    options = {'with_qt':           [True, False],
               'build_application': [True, False],
               'build_backend':     [True, False],
               'build_frontend':    [True, False],
               'build_tests':       [True, False]}

    default_options = {'with_qt':             False,
                       'build_application':   True,
                       'build_backend':       False,
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
            self.requires('restinio/0.6.13')

        if self.options.build_application and self.options.build_tests:
            self.requires('gtest/1.10.0')

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
