#!/usr/bin/env python3

import os
import subprocess
from pathlib import Path
from shutil import which

from env_validator import ToolsValidator

REQUIRED_CMAKE_VERSION = (4, 0, 0)


CLANG_VERSION = "20"
CONAN_VERSION = "2.16.1"


def run(cmd: str) -> None:
    print(f"Running: {cmd}")
    subprocess.check_call(cmd, shell=True)


def ensure_apt(pkg: str) -> None:
    run("apt-get update")
    run(f"apt-get install -y {pkg}")


def ensure_pip(pkg: str) -> None:
    run(f"pip install {pkg}")


def ensure_git() -> None:
    if which("git") is None:
        ensure_apt("git")


def ensure_conan() -> None:
    if which("conan") is None:
        ensure_pip(f"conan=={CONAN_VERSION}")


def ensure_cmake() -> None:
    def parse_version(output: str):
        version_str = output.split()[2]
        return tuple(int(v) for v in version_str.split("."))

    current = None
    if which("cmake") is not None:
        try:
            out = subprocess.check_output(["cmake", "--version"], text=True)
            current = parse_version(out.splitlines()[0])
        except Exception:
            current = None

    if current is None or current < REQUIRED_CMAKE_VERSION:
        ensure_snap()
        run("snap install cmake --classic")


def ensure_ninja() -> None:
    if which("ninja") is None:
        ensure_apt("ninja-build")


def ensure_snap() -> None:
    if which("snap") is None:
        ensure_apt("snapd")
        run("snap install core")
        run("snap refresh core")


def ensure_clang() -> None:
    if which("clang-20") is None and which("clang") is None:
        run("wget -q https://apt.llvm.org/llvm.sh")
        run("chmod +x llvm.sh")
        run(f"./llvm.sh {CLANG_VERSION} all")
        Path("llvm.sh").unlink()


def ensure_qt() -> None:
    qt_root = os.environ.setdefault("QT_ROOT", "/usr/lib/x86_64-linux-gnu")
    if not Path(qt_root).exists():
        ensure_apt("qt6-base-dev")
    qt_version_root = Path(qt_root) / "qt6"
    gcc_path = qt_version_root / "gcc_64"
    if not gcc_path.exists():
        gcc_path.mkdir(parents=True, exist_ok=True)
        (gcc_path / "bin").symlink_to(Path("../../../qt6"))
        (gcc_path / "libexec").symlink_to(Path("../../../qt6"))
        (gcc_path / "lib").symlink_to(Path("../../.."))
        (gcc_path / "plugins").symlink_to(Path("../plugins"))
        (gcc_path / "qml").symlink_to(Path("../qml"))
        (gcc_path / "mkspecs").symlink_to(Path("../mkspecs"))




def main() -> None:
    try:
        ensure_git()
        ensure_conan()
        ensure_cmake()
        ensure_ninja()
        ensure_clang()
        ensure_qt()

        ToolsValidator().check_all()
        print("Environment validated")
    except Exception as exc:
        print(f"bootstrap error: {exc}")


if __name__ == "__main__":
    main()
