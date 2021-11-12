from conans import ConanFile, CMake


class TermGraphConan(ConanFile):

    generators = 'cmake_find_package', 'cmake_paths'

    requires = 'gtest/1.10.0', 'restinio/0.6.13', 'outcome/2.2.1'

    options = {'with_qt':       [True, False],
               'build_backend': [True, False]}

    default_options = {'with_qt': False,
                       'build_backend': False,
                       'qt:qtdeclarative': True,
                       'qt:qtquickcontrols2': True,
                       'qt:qtsvg': True,
                       'qt:qtimageformats': True}

    def requirements(self):
        if self.options.with_qt:
            self.requires('qt/6.2.1')

        if self.options.build_backend:
            self.requires('restinio/0.6.13')

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
