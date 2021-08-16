from conans import ConanFile, CMake


class TermGraphConan(ConanFile):

    generators = 'cmake_find_package', 'cmake_paths'

    requires = 'gtest/1.10.0', 'restinio/0.6.13'

    options = {'with_qt': [True, False]}

    default_options = {'with_qt': False,
                       'qt:qtdeclarative': True,
                       'qt:qtquickcontrols2': True,
                       'qt:qtsvg': True,
                       'qt:qtimageformats': True}

    def requirements(self):
        if self.options.with_qt:
            self.requires('qt/6.1.2')

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
