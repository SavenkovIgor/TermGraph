#!/usr/bin/env python3

import os
import subprocess
import argparse
from pathlib import Path
from typing import List

def repository_root() -> Path:
    return Path(__file__).parent

def run(command: str):
    if subprocess.call(command, shell=True, executable='/bin/bash') != 0:
        exit(1)

def delete_if_exist(path: Path):
    if path.exists():
        print(f'Delete {path}')
        run(f'rm -rf {path}')

def env_qt_version() -> str:
    return os.environ['QT_VERSION']

def configure_environment(for_wasm: bool = False):
    os.environ.setdefault('QT_ROOT', os.path.expanduser('~/Qt'))
    os.environ.setdefault('QT_VERSION', '6.6.1')
    assert Path(os.environ['QT_ROOT']).exists(), 'Error: path at QT_ROOT env.variable not exist'

    if for_wasm:
        assert 'EMSDK' in os.environ, 'Error: EMSDK is not defined. Call source ~/emsdk/emsdk_env.sh or similar'
        assert Path(os.environ['EMSDK']).exists(), 'Error: path at EMSDK env.variable not exist'


class Project:
    def __init__(self, name: str, run_name: str, path: Path, available_presets: list[str]):
        self.name = name
        self.path = path
        self.run_name = run_name
        assert path.exists(), f'Error: path not exist {path}'
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
        args = [f'--profile=conanfiles/profile/{preset_name}']
        args += ['--build=missing']
        args += [f'-of={self.build_dir(preset_name)}/conan-dependencies']
        run(f'conan install . {" ".join(args)}')

    def configure(self, preset_name: str):
        self.prepare(preset_name)
        print(f'---CONFIGURE {self.name} with preset {preset_name}---')
        self.delete_cmake_user_presets()
        run(f'cmake --preset {preset_name} ./')

    def build(self, preset_name: str):
        self.prepare(preset_name)

        self.install(preset_name)
        self.configure(preset_name)

        print(f'---BUILD {self.name} preset: {preset_name}, Qt: {env_qt_version()}---')
        run(f'cmake --build --preset {preset_name}')

    def test(self, preset_name: str):
        self.prepare(preset_name)
        print(f'---TEST {self.name} with preset {preset_name}---')
        args: List[str] = []
        args.append(f'--preset {preset_name}')
        args.append('--output-on-failure')
        args.append('--verbose')
        args.append('--output-junit ctest.xml')
        run(f'ctest {" ".join(args)}')

    def run(self, preset_name: str):
        self.prepare(preset_name)
        print(f'---RUN {self.name} with preset {preset_name}---')
        run(f'./build/{preset_name}/{self.run_name}')

    def pack(self, preset_name: str):
        print('---CMAKE PACKAGE STARTED---')
        print('Not implemented yet')

    def clear(self, clear_conan: bool = False):
        os.chdir(self.path)
        print(f'---CLEAR {self.name}---')
        delete_if_exist(self.path / 'build')
        if clear_conan:
            run('conan remove -c "*"')


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
    parser.add_argument('--rebuild',     action='store_true', help='Rebuild project (clear, configure, build)')

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

    if args.rebuild:
        app.clear()
        app.install(args.preset)
        app.configure(args.preset)
        app.build(args.preset)
        app.run(args.preset)


if __name__ == '__main__':
    main()
