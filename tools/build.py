#!/usr/bin/env python3

import os, argparse
from pathlib import Path

def repository_root() -> Path:
    return Path(__file__).parent.parent

def set_env_var_if_missed(env_var_name: str, default_value: str):
    if env_var_name not in os.environ:
        os.environ[env_var_name] = default_value
        print(f'Env.variable {env_var_name} not found. Set to {default_value}')


def assert_variable_exist(varname: str, example_msg: str):
  if not varname in os.environ:
    print(f'Error: variable not defined {varname}')
    print(f'Can be defined as: {example_msg}')
    exit(1)


def assert_path_exist(path: Path, comment: str):
  if not path.exists():
    print(f'Error: path not exist {comment} {path}')
    exit(1)

def assert_system_call(command: str):
  if os.system(command) != 0:
    exit(1)

class QtFolders:
    qt_root = Path('')
    qt_version = ''

    @classmethod
    def default(cls):
        return cls(Path(os.environ['QT_ROOT']), os.environ['QT_VERSION'])

    def __init__(self, qt_root: Path, qt_version: str):
        self.qt_root = qt_root
        self.qt_version = qt_version

    def root(self) -> Path:
        return self.qt_root

    def version(self) -> str:
        return self.qt_version

    def version_dir(self) -> Path:
        return self.root() / self.qt_version

    def source(self) -> Path:
        return self.version_dir() / 'Src'

    def toolchain_gcc_64(self) -> Path:
        return self.version_dir() / 'gcc_64'


def configure_environment(for_wasm: bool = False):
    set_env_var_if_missed('QT_ROOT', os.path.expanduser('~/Qt'))
    set_env_var_if_missed('QT_VERSION', '6.4.2')

    assert_variable_exist('QT_VERSION', '6.4.2')
    assert_variable_exist('QT_ROOT', '~/Qt')
    assert_path_exist(Path(os.environ['QT_ROOT']), 'from variable QT_ROOT')

    if for_wasm:
        assert_system_call(f'source {os.path.expanduser("~/emsdk/emsdk_env.sh")}')

        assert_variable_exist('EMSDK', '~/emsdk')
        assert_variable_exist('EMSDK_NODE', '~/emsdk/node/14.18.2_64bit/bin/node')
        assert_path_exist(Path(os.environ['EMSDK']), 'from variable EMSDK')
        assert_path_exist(Path(os.environ['EMSDK_NODE']), 'from variable EMSDK_NODE')


def build_multithread_wasm():
    print('---Build multithread wasm---')

    configure_environment(for_wasm=True)

    qt_sdk = QtFolders.default()

    CONFIG_PLATFORM  = 'wasm-emscripten'
    CONFIG_HOST_PATH = qt_sdk.toolchain_gcc_64()
    CONFIG_MODULES   = 'qtbase,qtsvg,qtimageformats,qtshadertools,qtdeclarative,qt5compat'
    THREAD_ARG = '-feature-thread'

    print('---Cloning---')

    assert_system_call('git clone git://code.qt.io/qt/qt5.git ./qt_src')
    os.chdir('./qt_src')

    print('---Checkout---')
    assert_system_call(f'git checkout v{qt_sdk.version()}')

    print('---Init-repository---')
    assert_system_call('perl init-repository')

    THREAD_ARG = '-feature-thread'

    print('---Configure---')
    os.system('rm -rf qtwebengine')

    assert_system_call(f"./configure -qt-host-path {CONFIG_HOST_PATH} -platform {CONFIG_PLATFORM} {THREAD_ARG} -prefix {qt_sdk.version_dir()}/wasm_32_mt -submodules {CONFIG_MODULES} -skip 'qtwebengine'")

    print('---Build---')
    assert_system_call('cmake --build . --parallel')
    assert_system_call('cmake --install .')


def delete_cmake_user_presets():
    if os.path.exists('CMakeUserPresets.json'):
        os.remove('CMakeUserPresets.json')


def build_app(preset_name: str):
    print('---CONAN INSTALL STARTED---')
    assert_system_call(f'conan install . --profile conanfiles/profile/{preset_name} --build=missing -if=build/{preset_name}/conan-dependencies')

    print('---CMAKE CONFIGURE STARTED---')
    assert_system_call(f'cmake --preset {preset_name} ./')

    print('---CMAKE BUILD STARTED---')
    assert_system_call(f'cmake --build --preset {preset_name}')


def test_app(preset_name: str):
    print('---CMAKE TEST STARTED---')
    assert_system_call(f'ctest --preset {preset_name}')


def main():
    parser = argparse.ArgumentParser(description='Build application with CMake preset')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--clear', help='Clean build directory', action='store_true')
    group.add_argument('--wasm-multithread', help='Build multithread wasm', action='store_true')
    group.add_argument('preset_name',
                        type=str,
                        help='CMake preset name',
                        nargs='?',
                        choices=['desktop_release', 'desktop_dev', 'wasm_release'])
    args = parser.parse_args()

    if args.wasm_multithread:
        build_multithread_wasm()

    elif args.clear:
        os.chdir(repository_root() / 'apps/Application')

        delete_cmake_user_presets()
        os.system('rm -rf ./build')

    else:
        os.chdir(repository_root() / 'apps/Application')

        configure_environment(args.preset_name == 'wasm_release')
        delete_cmake_user_presets()
        build_app(args.preset_name)

        if args.preset_name != 'wasm_release':
            test_app(args.preset_name)

if __name__ == '__main__':
    main()
