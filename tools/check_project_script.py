import subprocess

# Should be possible to run:
# ./project.py --target-list # List all targets
# ./project.py --install   [Application (default) | Backend] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --configure [Application (default) | Backend] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --build     [Application (default) | Backend | QtWasmMultithread] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --run       [Application (default) | Backend] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --test      [Application (default) | Backend] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --pack      [Application (default) | Backend] [--preset desktop_release (default) | desktop_dev | wasm_release]
# ./project.py --clear     [Application (default) | Backend]
# ./project.py --clear-all [Application (default) | Backend]


def assert_system_call(command: str):
    print(f'Running command: {command}')
    if subprocess.call(command, shell=True, executable='/bin/bash') != 0:
        exit(1)

    print('')


def test_project_script():
    assert_system_call('./project.py --help')
    assert_system_call('./project.py --target-list')

    assert_system_call('./project.py --install Application')
    assert_system_call('./project.py --install Application --preset desktop_dev')
    assert_system_call('./project.py --install Application --preset wasm_release')
    assert_system_call('./project.py --install Backend')
    assert_system_call('./project.py --install')

    assert_system_call('./project.py --configure Application --preset desktop_dev')
    assert_system_call('./project.py --configure Backend --preset desktop_release')
    assert_system_call('./project.py --configure')

    assert_system_call('./project.py --build Application --preset=desktop_dev')
    assert_system_call('./project.py --build Backend --preset desktop_release')
    # assert_system_call('./project.py --build QtWasmMultithread')
    assert_system_call('./project.py --build')

    assert_system_call('./project.py --test Application --preset=desktop_dev')
    assert_system_call('./project.py --test Backend --preset desktop_release')
    assert_system_call('./project.py --test')

    assert_system_call('./project.py --run')

    assert_system_call('./project.py --pack Application --preset=desktop_dev')
    assert_system_call('./project.py --pack Backend --preset desktop_release')
    assert_system_call('./project.py --pack')

    assert_system_call('./project.py --clear Application')
    assert_system_call('./project.py --clear Backend')
    assert_system_call('./project.py --clear')
    assert_system_call('./project.py --clear-all Application')
    assert_system_call('./project.py --clear-all Backend')
    assert_system_call('./project.py --clear-all')


if __name__ == '__main__':
    test_project_script()
