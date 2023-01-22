import os
from pathlib import Path

def assert_variable_exist(varname: str, example_msg: str):
  if not varname in os.environ:
    print(f'Error: variable not defined {varname}')
    print(f'Can be defined as: {example_msg}')
    exit(1)

def assert_path_exist(path: Path, comment: str):
  if not path.exists():
    print(f'Error: path not exist {comment} {path}')
    exit(1)

def check_emsdk_env():
  # If fails here, exec emscripten evn script manually!
  # source '$HOME/emsdk/emsdk_env.sh'
  assert_variable_exist('EMSDK', '~/emsdk')
  assert_variable_exist('EMSDK_NODE', '~/emsdk/node/14.18.2_64bit/bin/node')
  assert_path_exist(Path(os.environ['EMSDK']), 'from variable EMSDK')
  assert_path_exist(Path(os.environ['EMSDK_NODE']), 'from variable EMSDK_NODE')

def check_qt_env():
  assert_variable_exist('QT_VERSION', '6.4.2')
  assert_variable_exist('QT_ROOT', '~/Qt')
  assert_path_exist(Path(os.environ['QT_ROOT']), 'from variable QT_ROOT')


check_emsdk_env()
check_qt_env()

QT_ROOT    = Path(os.environ['QT_ROOT'])
QT_VERSION = os.environ['QT_VERSION']

QT_VERSION_PATH = QT_ROOT / QT_VERSION
assert_path_exist(QT_VERSION_PATH, 'from variable QT_VERSION_PATH')

CONFIG_PLATFORM  = 'wasm-emscripten'
CONFIG_HOST_PATH = QT_VERSION_PATH / 'gcc_64'
CONFIG_MODULES   = 'qtbase,qtsvg,qtimageformats,qtshadertools,qtdeclarative'

print('---Cloning---')
os.chdir('..')
os.system('git clone git://code.qt.io/qt/qt5.git ./source')
os.chdir('./source')

print('---Checkout---')
os.system(f'git checkout v{QT_VERSION}')

print('---Init-repository---')
os.system('perl init-repository')

THREAD_ARG = '-feature-thread'

print('---Configure---')
os.system('rm -rf qtwebengine')

os.system(f"./configure -qt-host-path {CONFIG_HOST_PATH} -platform {CONFIG_PLATFORM} {THREAD_ARG} -prefix {QT_VERSION_PATH}/wasm_32_mt -submodules {CONFIG_MODULES} -skip 'qtwebengine'")

print('---Build---')
os.system('cmake --build . --parallel 4')
os.system('cmake --install .')
