import subprocess

# Should be possible to run:
# ./project.py --deps-install [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --configure    [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --build        [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --cmake-install [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --run          [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --test         [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --pack         [Application (default) ] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --clear        [Application (default) ]
# ./project.py --clear-all    [Application (default) ]


def assert_system_call(command: str):
    print(f'Running command: {command}')
    if subprocess.call(command, shell=True, executable='/bin/bash') != 0:
        exit(1)

    print('')


def test_project_script():
    assert_system_call('./project.py --help')

    assert_system_call('./project.py --deps-install')
    assert_system_call('./project.py --deps-install --preset desktop_dev')
    assert_system_call('./project.py --deps-install --preset wasm_release')

    assert_system_call('./project.py --configure --preset desktop_dev')
    assert_system_call('./project.py --configure')

    assert_system_call('./project.py --build --preset=desktop_dev')
    assert_system_call('./project.py --build')

    assert_system_call('./project.py --cmake-install --preset=desktop_dev')
    assert_system_call('./project.py --cmake-install')

    assert_system_call('./project.py --test --preset=desktop_dev')
    assert_system_call('./project.py --test')

    assert_system_call('./project.py --run')

    assert_system_call('./project.py --pack --preset=desktop_dev')
    assert_system_call('./project.py --pack')

    assert_system_call('./project.py --clear')
    assert_system_call('./project.py --clear-all')


if __name__ == '__main__':
    test_project_script()
