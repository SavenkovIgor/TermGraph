#!/usr/bin/env python3

import os
import sys
import subprocess
import argparse
from pathlib import Path


def repository_root() -> Path:
    return Path(__file__).parent


def set_env_var_if_missed(env_var_name: str, default_value: str):
    if env_var_name not in os.environ:
        os.environ[env_var_name] = default_value
        print(f'Env.variable {env_var_name} not found. Set to {default_value}')


def assert_variable_exist(varname: str, example_msg: str):
    if not varname in os.environ:
        print(f'Error: variable not defined {varname}')
        print(f'Can be defined as: {example_msg}')
        exit(1)


def is_valid_env_path(varname: str) -> bool:
    return varname in os.environ and Path(os.environ[varname]).exists()


def assert_path_exist(path: Path, comment: str):
    if not path.exists():
        print(f'Error: path not exist {comment} {path}')
        exit(1)


def assert_system_call(command: str):
    if subprocess.call(command, shell=True, executable='/bin/bash') != 0:
        exit(1)


def delete_if_exist(path: Path):
    if path.exists():
        print(f'Delete {path}')
        assert_system_call(f'rm -rf {path}')


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
    set_env_var_if_missed('QT_VERSION', 'It must be in format x.y.z')

    assert_variable_exist('QT_VERSION', 'x.y.z')
    assert_variable_exist('QT_ROOT', '~/Qt')
    assert_path_exist(Path(os.environ['QT_ROOT']), 'from variable QT_ROOT')

    if for_wasm:
        has_emsdk = is_valid_env_path('EMSDK')
        has_emsdk_node = is_valid_env_path('EMSDK_NODE')

        if not has_emsdk or not has_emsdk_node:
            assert_system_call(f'source {os.path.expanduser("~/emsdk/emsdk_env.sh")}')

        assert_variable_exist('EMSDK', '~/emsdk')
        assert_variable_exist('EMSDK_NODE', '~/emsdk/node/14.18.2_64bit/bin/node')
        assert_path_exist(Path(os.environ['EMSDK']), 'from variable EMSDK')
        assert_path_exist(Path(os.environ['EMSDK_NODE']), 'from variable EMSDK_NODE')


def build_multithread_wasm():
    print('---Build multithread wasm---')

    configure_environment(for_wasm=True)

    qt_sdk = QtFolders.default()

    CONFIG_PLATFORM = 'wasm-emscripten'
    CONFIG_HOST_PATH = qt_sdk.toolchain_gcc_64()
    CONFIG_MODULES = 'qtbase,qtsvg,qtimageformats,qtshadertools,qtdeclarative,qt5compat'
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
    assert_system_call('rm -rf qtwebengine')

    assert_system_call(
        f"./configure -qt-host-path {CONFIG_HOST_PATH} -platform {CONFIG_PLATFORM} {THREAD_ARG} -prefix {qt_sdk.version_dir()}/wasm_multithread -submodules {CONFIG_MODULES} -skip 'qtwebengine'")

    print('---Build---')
    assert_system_call('cmake --build . --parallel')
    assert_system_call('cmake --install .')


class Project:
    def __init__(self, name: str, run_name: str, path: Path, available_presets: list[str]):
        self.name = name
        self.path = path
        self.run_name = run_name
        assert_path_exist(path, 'from project path')
        self.available_presets = available_presets

    def project_dir(self) -> Path:
        return self.path

    def build_dir(self, preset_name: str) -> Path:
        return self.path / f'build/{preset_name}'

    def delete_cmake_user_presets(self):
        delete_if_exist(self.project_dir() / 'CMakeUserPresets.json')

    def check_preset(self, preset_name: str):
        if not preset_name in self.available_presets:
            print(f'Preset {preset_name} not found in {self.name}')
            exit(1)

    def prepare(self, preset_name: str):
        self.check_preset(preset_name)
        os.chdir(self.path)

    def install(self, preset_name: str):
        self.prepare(preset_name)
        print(f'---INSTALL {self.name} with preset {preset_name}---')
        assert_system_call(
            f'conan install . --profile conanfiles/profile/{preset_name} --build=missing -if={self.build_dir(preset_name)}/conan-dependencies')

    def configure(self, preset_name: str):
        self.prepare(preset_name)
        print(f'---CONFIGURE {self.name} with preset {preset_name}---')
        self.delete_cmake_user_presets()
        assert_system_call(f'cmake --preset {preset_name} ./')

    def build(self, preset_name: str):
        self.prepare(preset_name)

        self.install(preset_name)
        self.configure(preset_name)

        print(f'---BUILD {self.name} with preset {preset_name}---')
        assert_system_call(f'cmake --build --preset {preset_name}')

    def test(self, preset_name: str):
        self.prepare(preset_name)
        print(f'---TEST {self.name} with preset {preset_name}---')
        assert_system_call(f'ctest --preset {preset_name} --output-on-failure')

    def run(self, preset_name: str):
        self.prepare(preset_name)
        print(f'---RUN {self.name} with preset {preset_name}---')
        assert_system_call(f'./build/{preset_name}/{self.run_name}')

    def pack(self, preset_name: str):
        print('---CMAKE PACKAGE STARTED---')
        print('Not implemented yet')

    def clear(self, clear_conan: bool = False):
        os.chdir(self.path)
        print(f'---CLEAR {self.name}---')
        delete_if_exist(self.path / 'build')
        if clear_conan:
            assert_system_call('conan remove -f "*"')


# Should be possible to run:
# ./project.py --target-list # List all targets
# ./project.py --install   [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --configure [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --build     [Application (default) | QtWasmMultithread] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --run       [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --test      [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --pack      [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --clear     [Application (default) ]
# ./project.py --clear-all [Application (default) ]
def main():
    parser = argparse.ArgumentParser(description='Project build script')

    parser.add_argument('--target-list', action='store_true', help='List all targets')
    parser.add_argument('--install',     action='store_true', help='Install dependencies')
    parser.add_argument('--configure',   action='store_true', help='Configure project')
    parser.add_argument('--build',       action='store_true', help='Build project')
    parser.add_argument('--test',        action='store_true', help='Test project')
    parser.add_argument('--run',         action='store_true', help='Run project')
    parser.add_argument('--pack',        action='store_true', help='Pack project')
    parser.add_argument('--clear',       action='store_true', help='Clear project')
    parser.add_argument('--clear-all',   action='store_true', help='Clear project and conan cache')

    parser.add_argument('target',   type=str, help='Target to build', choices=[
                        'Application', 'QtWasmMultithread'], default='Application', nargs='?')
    parser.add_argument('--preset', type=str, help='Preset to use',
                        choices=['desktop_dev', 'desktop_release', 'wasm_release'], default='desktop_release')

    args = parser.parse_args()

    if args.target_list:
        print('Available targets: Application, QtWasmMultithread')
        return

    if args.target == 'QtWasmMultithread':

        if not args.build:
            print('QtWasmMultithread can only be built')

        build_multithread_wasm()
        return

    configure_environment()

    app = Project('Application', 'TermGraph', repository_root(),
                  ['desktop_dev', 'desktop_release', 'wasm_release'])

    if args.install:
        app.install(args.preset)

    if args.configure:
        app.configure(args.preset)

    if args.build:
        app.build(args.preset)

    if args.test:
        app.test(args.preset)

    if args.run:
        app.run(args.preset)

    if args.pack:
        app.pack(args.preset)

    if args.clear:
        app.clear()

    if args.clear_all:
        app.clear(clear_conan=True)


if __name__ == '__main__':
    main()
