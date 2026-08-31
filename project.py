#!/usr/bin/env python3

import argparse
import json
import logging
import os
import re
import subprocess
import sys
from pathlib import Path
from typing import Any

log_config: dict[str, Any] = {
    'level': 'INFO',
    'format': '%(levelname)s: %(message)s',
}

logging.basicConfig(**log_config)

REPOSITORY_ROOT = Path(__file__).resolve().parent


def load_presets() -> list[str]:
    cmake_presets: dict[str, Any]
    with (REPOSITORY_ROOT / 'CMakePresets.json').open(encoding='utf-8') as file:
        cmake_presets = json.load(file)

    return [ preset['name'] for preset in cmake_presets['configurePresets'] if not preset.get('hidden', False) ]

def resolved_profile_text(profile_path: Path, _seen: set[Path] | None = None) -> str:
    """Read a Conan profile, inlining `include(...)` references, which may be a bare name
    resolved next to the including file (e.g. 'desktop_dev') or a relative path (e.g. '../base/clang')."""
    profile_path = profile_path.resolve()
    _seen = _seen if _seen is not None else set()
    if profile_path in _seen:
        return ''
    _seen.add(profile_path)

    text = profile_path.read_text(encoding='utf-8')
    for included in re.findall(r'include\(([^)]+)\)', text):
        text += resolved_profile_text(profile_path.parent / included, _seen)
    return text

def is_wasm_preset(preset: str) -> bool:
    profile_path = REPOSITORY_ROOT / f'conanfiles/profile/host/{preset}'
    return 'os=Emscripten' in resolved_profile_text(profile_path)

def run(command: str):
    if subprocess.call(command, shell=True, executable='/bin/bash') != 0:
        exit(1)

def delete_if_exist(path: Path):
    if path.exists():
        logging.info(f'Delete {path}')
        run(f'rm -rf {path}')

def env_qt_version() -> str:
    return os.environ['QT_VERSION']

def configure_environment(for_wasm: bool = False):
    os.environ.setdefault('QT_ROOT', os.path.expanduser('~/Qt'))
    os.environ.setdefault('QT_VERSION', '6.11.1')
    os.environ.setdefault('QT_VERSION_ROOT', f"{os.environ['QT_ROOT']}/{os.environ['QT_VERSION']}")
    assert Path(os.environ['QT_ROOT']).exists(), 'Error: path at QT_ROOT env.variable not exist'

    if for_wasm:
        assert 'EMSDK' in os.environ, 'Error: EMSDK is not defined. Call source ~/emsdk/emsdk_env.sh or similar'
        assert Path(os.environ['EMSDK']).exists(), 'Error: path at EMSDK env.variable not exist'


class Project:
    def __init__(self, name: str, run_name: str, path: Path):
        self.name = name
        self.path = path
        self.run_name = run_name
        assert path.exists(), f'Error: path not exist {path}'
        os.chdir(self.path)

    def build_dir(self, preset: str) -> Path:
        return self.path / f'build/{preset}'

    def conan_build_env(self, preset: str) -> Path:
        return self.build_dir(preset) / 'conan-dependencies/conanbuild.sh'

    def conan_env_prefix(self, preset: str) -> str:
        return f'source {self.conan_build_env(preset)}'

    def init_submodules(self, force: bool = False):
        """Initialize and update git submodules"""
        # Early exit
        stemming_path = REPOSITORY_ROOT / 'third_party/stemming/src'
        if not force and stemming_path.exists() and stemming_path.is_dir():
            logging.info('---GIT SUBMODULES ALREADY INITIALIZED---')
            return

        os.chdir(REPOSITORY_ROOT)
        logging.info('---INITIALIZING GIT SUBMODULES---')
        run('git submodule update --init --recursive')
        logging.info('Git submodules initialized successfully')
        os.chdir(self.path)

    def install_conan_deps(self, preset: str, force: bool = False):
        # Early exit
        if not force and self.conan_build_env(preset).exists():
            logging.info(f'---DEPS ALREADY INSTALLED {self.name} with preset {preset}---')
            return

        logging.info(f'---DEPS INSTALL {self.name} with preset {preset}---')
        args = [f'--profile:host=conanfiles/profile/host/{preset}']
        args += ['--profile:build=conanfiles/profile/build/build_machine']
        args += ['--build=missing']
        args += [f'-of={self.build_dir(preset)}/conan-dependencies']
        args += ['--deployer=conanfiles/deployers/tool_requires_deploy.py']
        args += [f'--deployer-folder={REPOSITORY_ROOT}/.conan-tools']
        run(f'conan install . {' '.join(args)}')

    def configure_cmake(self, preset: str, force: bool = False):
        # Early exit
        cmake_cache_path = self.build_dir(preset) / 'CMakeCache.txt'
        if not force and cmake_cache_path.exists():
            logging.info(f'---CMAKE ALREADY CONFIGURED {self.name} with preset {preset}---')
            return

        logging.info(f'---CONFIGURE {self.name} with preset {preset}---')
        run(f'{self.conan_env_prefix(preset)} && cmake --preset {preset}')

    def bootstrap(self, preset: str, force: bool = False):
        """Initialize submodules, install deps and run cmake configure, but only when actually needed"""
        self.init_submodules(force)
        self.install_conan_deps(preset, force)
        self.configure_cmake(preset, force)

    def cmake_install(self, preset: str):
        logging.info(f'---CMAKE INSTALL {self.name} with preset {preset}---')
        run(f'{self.conan_env_prefix(preset)} && cmake --install {self.build_dir(preset)}')

    def build(self, preset: str):
        self.bootstrap(preset)

        logging.info(f'---BUILD {self.name} preset: {preset}, Qt: {env_qt_version()}---')
        run(f'{self.conan_env_prefix(preset)} && cmake --build --preset {preset}')

    def test(self, preset: str):
        logging.info(f'---TEST {self.name} with preset {preset}---')
        args: list[str] = []
        args.append(f'--preset {preset}')
        args.append('--output-on-failure')
        args.append('--verbose')
        args.append('--output-junit ctest.xml')
        run(f'{self.conan_env_prefix(preset)} && ctest {' '.join(args)}')

    def run(self, preset: str):
        logging.info(f'---RUN {self.name} with preset {preset}---')
        run(f'./build/{preset}/{self.run_name}')

    def pack(self, preset: str):
        logging.info(f'---CMAKE PACKAGE STARTED---')
        logging.error('Not implemented yet')

    def clear(self, clear_conan: bool = False):
        logging.info(f'---CLEAR {self.name}---')
        delete_if_exist(self.path / 'build')
        if clear_conan:
            run('conan remove -c "*"')

def main(args: argparse.Namespace):
    is_wasm = args.preset is not None and is_wasm_preset(args.preset)
    configure_environment(for_wasm=is_wasm)

    app = Project('Application', 'TermGraph', REPOSITORY_ROOT)

    if args.bootstrap:
        app.bootstrap(args.preset, force=True)

    if args.build:
        app.build(args.preset)

    if args.cmake_install:
        app.cmake_install(args.preset)

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
        app.bootstrap(args.preset, force=True)
        app.build(args.preset)
        app.run(args.preset)


# Should be possible to run:
# ./project.py --bootstrap        [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --build            [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --cmake-install    [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --run              [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --test             [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --pack             [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --clear
# ./project.py --clear-all
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Project build script')

    parser.add_argument('--bootstrap',     action='store_true', help='Initialize submodules, install dependencies and configure CMake')
    parser.add_argument('--build',         action='store_true', help='Build project')
    parser.add_argument('--cmake-install', action='store_true', help='Install project (cmake install)')
    parser.add_argument('--test',          action='store_true', help='Test project')
    parser.add_argument('--run',           action='store_true', help='Run project')
    parser.add_argument('--pack',          action='store_true', help='Pack project')
    parser.add_argument('--clear',         action='store_true', help='Clear project')
    parser.add_argument('--clear-all',     action='store_true', help='Clear project and conan cache')
    parser.add_argument('--rebuild',       action='store_true', help='Rebuild project (clear, configure, build)')

    parser.add_argument('--preset', type=str, help='Preset to use', choices=load_presets(), default='desktop_release')

    args = parser.parse_args()
    sys.exit(main(args))
