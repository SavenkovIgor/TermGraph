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


def assert_variable_exist(varname: str, example: str):
    if not varname in os.environ:
        print(f'Error: variable not defined {varname}')
        print(f'Can be defined as: {example}')
        exit(1)


def is_valid_env_path(varname: str) -> bool:
    return varname in os.environ and Path(os.environ[varname]).exists()


def assert_path_exist(path: Path, error: str):
    if not path.exists():
        print(f'Error: path not exist {path}')
        print({error})
        exit(1)


def assert_system_call(command: str):
    if subprocess.call(command, shell=True, executable='/bin/bash') != 0:
        exit(1)


def delete_if_exist(path: Path):
    if path.exists():
        print(f'Delete {path}')
        assert_system_call(f'rm -rf {path}')


def env_qt_version() -> str:
    return os.environ['QT_VERSION']


def env_qt_root() -> Path:
    return Path(os.environ['QT_ROOT'])


def configure_environment(for_wasm: bool = False):
    set_env_var_if_missed('QT_ROOT',    default_value=os.path.expanduser('~/Qt'))
    set_env_var_if_missed('QT_VERSION', default_value='6.5.0')

    assert_variable_exist('QT_VERSION', example='x.y.z')
    assert_variable_exist('QT_ROOT',    example='~/Qt')
    assert_path_exist(env_qt_root(), error='Path from QT_ROOT env.variable')

    if for_wasm:
        has_emsdk = is_valid_env_path('EMSDK')
        has_emsdk_node = is_valid_env_path('EMSDK_NODE')

        if not has_emsdk or not has_emsdk_node:
            assert_system_call(f'source {os.path.expanduser("~/emsdk/emsdk_env.sh")}')

        assert_variable_exist('EMSDK',      example='~/emsdk')
        assert_variable_exist('EMSDK_NODE', example='~/emsdk/node/14.18.2_64bit/bin/node')
        assert_path_exist(Path(os.environ['EMSDK']),      error='Path from EMSDK env.variable')
        assert_path_exist(Path(os.environ['EMSDK_NODE']), error='Path from EMSDK_NODE env.variable')


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

        print(f'---BUILD {self.name} preset: {preset_name}, Qt: {env_qt_version()}---')
        assert_system_call(f'cmake --build --preset {preset_name}')

    def test(self, preset_name: str):
        self.prepare(preset_name)
        print(f'---TEST {self.name} with preset {preset_name}---')
        assert_system_call(f'ctest --preset {preset_name} --output-on-failure --verbose')

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
# ./project.py --install   [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --configure [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --build     [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --run       [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --test      [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --pack      [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --clear
# ./project.py --clear-all
def main():
    parser = argparse.ArgumentParser(description='Project build script')

    parser.add_argument('--install',     action='store_true', help='Install dependencies')
    parser.add_argument('--configure',   action='store_true', help='Configure project')
    parser.add_argument('--build',       action='store_true', help='Build project')
    parser.add_argument('--test',        action='store_true', help='Test project')
    parser.add_argument('--run',         action='store_true', help='Run project')
    parser.add_argument('--pack',        action='store_true', help='Pack project')
    parser.add_argument('--clear',       action='store_true', help='Clear project')
    parser.add_argument('--clear-all',   action='store_true', help='Clear project and conan cache')

    parser.add_argument('--preset', type=str, help='Preset to use',
                        choices=['desktop_dev', 'desktop_release', 'wasm_release'], default='desktop_release')

    args = parser.parse_args()

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
