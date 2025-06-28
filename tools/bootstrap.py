#!/usr/bin/env python3

import os
import subprocess
from pathlib import Path
from shutil import which

from env_validator import ToolsValidator
from typing import List

REQUIRED_CMAKE_VERSION = (4, 0, 0)


CLANG_VERSION = "20"
CONAN_VERSION = "2.16.1"


def run(cmd: str) -> None:
    print(f"Running: {cmd}")
    subprocess.check_call(cmd, shell=True)



class Apt:
    """Simple wrapper around apt commands."""

    def update(self) -> None:
        run("apt update")

    def install(self, packages: List[str]) -> None:
        pkg_str = " ".join(packages)
        run(f"apt install -y {pkg_str}")

    def remove(self, packages: List[str]) -> None:
        pkg_str = " ".join(packages)
        run(f"apt remove -y {pkg_str}")


apt = Apt()

def ensure_apt(pkg: str) -> None:
    apt.update()
    apt.install([pkg])


def ensure_pip(pkg: str) -> None:
    run(f"pip install {pkg}")


def ensure_git() -> None:
    if which("git") is None:
        ensure_apt("git")


def ensure_conan() -> None:
    if which("conan") is None:
        ensure_pip(f"conan=={CONAN_VERSION}")


def install_cmake_from_kitware() -> None:
    apt.remove(["cmake"])
    apt.update()
    apt.install(["wget", "gpg", "ca-certificates"])
    run("wget -O - https://apt.kitware.com/kitware-archive.sh | bash")
    apt.update()
    apt.install(["cmake"])


def ensure_cmake() -> None:
    """Always install the latest CMake from the Kitware repository."""
    install_cmake_from_kitware()


def ensure_ninja() -> None:
    if which("ninja") is None:
        ensure_apt("ninja-build")


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
