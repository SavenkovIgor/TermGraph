#!/usr/bin/env python3

import os, argparse


def set_env_var_if_missed(env_var_name: str, default_value: str):
    if env_var_name not in os.environ:
        os.environ[env_var_name] = default_value
        print(f'Env.variable {env_var_name} not found. Set to {default_value}')

def configure_environment(preset_name: str):
    set_env_var_if_missed('QT_ROOT', os.path.expanduser('~/Qt'))
    set_env_var_if_missed('QT_VERSION', '6.4.2')

    if preset_name == 'wasm_release':
        os.system(f'source {os.path.expanduser("~/emsdk/emsdk_env.sh")}')

def to_application_dir():
    os.chdir(os.path.expanduser('../apps/Application'))

def delete_cmake_user_presets():
    if os.path.exists('CMakeUserPresets.json'):
        os.remove('CMakeUserPresets.json')

def clear_build():
    delete_cmake_user_presets()
    os.system(f'rm -rf ./build')

def conan_install(preset_name: str):
    print('---CONAN INSTALL STARTED---')
    os.system(f'conan install . --profile conanfiles/profile/{preset_name} --build=missing -if=build/{preset_name}/conan-dependencies')

def cmake_config(preset_name: str):
    print('---CMAKE CONFIGURE STARTED---')
    os.system(f'cmake --preset {preset_name} ./')

def cmake_build(preset_name: str):
    print('---CMAKE BUILD STARTED---')
    os.system(f'cmake --build --preset {preset_name}')

def build_app(preset_name: str):
    conan_install(preset_name)
    cmake_config(preset_name)
    cmake_build(preset_name)

def test_app(preset_name: str):
    print('---CMAKE TEST STARTED---')
    if preset_name != 'wasm_release':
        os.system(f'cmake --build --preset {preset_name} --target test')

def main():
    parser = argparse.ArgumentParser(description='Build application with CMake preset')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--clear', help='Clean build directory', action='store_true')
    group.add_argument('preset_name', 
                        type=str, 
                        help='CMake preset name', 
                        nargs='?',
                        choices=['desktop_release', 'desktop_dev', 'wasm_release'])
    args = parser.parse_args()

    to_application_dir()

    if args.clear:
        clear_build()
    else:
        configure_environment(args.preset_name)
        delete_cmake_user_presets()
        build_app(args.preset_name)
        test_app(args.preset_name)

if __name__ == '__main__':
    main()
