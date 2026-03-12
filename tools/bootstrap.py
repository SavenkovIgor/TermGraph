#!/usr/bin/env python3

import os
import subprocess
from pathlib import Path
from shutil import which
from typing import List

from env_validator import ToolsValidator

CLANG_VERSION = "20"
CONAN_VERSION = "2.25.2"
QT_VERSION = "6.8.3"
AQT_VERSION = "3.3.0"

# Base packages mirroring Dockerfile
BASE_PACKAGES = [
    "software-properties-common",
    "libgl1-mesa-dev",
    "libxkbcommon-dev",
    "libfontconfig1",
    "libfreetype6",
    "python3-pip",
    "python3-venv",
    "ca-certificates",
    "gpg",
    "wget",
    "unzip",
    "curl",
]


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


def ensure_uv() -> None:
    if which("uv") is None:
        run("curl -fsSL https://astral.sh/uv/install.sh -o /tmp/uv.sh")
        run("sh /tmp/uv.sh")
        Path("/tmp/uv.sh").unlink()


def ensure_base_packages() -> None:
    apt.update()
    apt.install(BASE_PACKAGES)

def ensure_git() -> None:
    if which("git") is None:
        ensure_apt("git")


def ensure_conan() -> None:
    if which("conan") is None:
        # Follow Dockerfile: use uv tool to install a specific conan version
        run(f"uv tool install conan@{CONAN_VERSION}")


def ensure_clang() -> None:
    if which(f"clang-{CLANG_VERSION}") is None:
        run("wget https://apt.llvm.org/llvm.sh")
        run("chmod +x llvm.sh")
        run(f"./llvm.sh {CLANG_VERSION} all")
        Path("llvm.sh").unlink()
        run(
            "update-alternatives --install /usr/bin/clang clang /usr/bin/clang-"
            f"{CLANG_VERSION} 100 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-{CLANG_VERSION}"
        )


def ensure_gdb() -> None:
    if which("gdb") is None:
        ensure_apt("gdb")


def ensure_qt() -> None:
    os.environ.setdefault("QT_VERSION", QT_VERSION)
    qt_root = Path(os.environ.setdefault("QT_ROOT", "/workspaces/Qt"))
    if not qt_root.exists():
        qt_root.mkdir(parents=True, exist_ok=True)
        run(
            "uvx --from aqtinstall@" + AQT_VERSION +
            f" aqt install-qt -O {qt_root} linux desktop {QT_VERSION} linux_gcc_64 "
            "--modules qtimageformats qtshadertools "
            "--archives qttranslations qttools qtsvg qtdeclarative qtbase icu"
        )
    persist_env_vars({"QT_ROOT": str(qt_root), "QT_VERSION": QT_VERSION})


def persist_env_vars(vars: dict) -> None:
    bashrc = Path.home() / ".bashrc"
    bashrc.touch(exist_ok=True)
    lines = bashrc.read_text().splitlines()
    for key, value in vars.items():
        export_line = f"export {key}={value}"
        if export_line not in lines:
            lines.append(export_line)
    bashrc.write_text("\n".join(lines) + "\n")




def main() -> None:
    try:
        ensure_base_packages()
        ensure_uv()
        ensure_git()
        ensure_conan()
        ensure_clang()
        ensure_gdb()
        ensure_qt()

        ToolsValidator().check_all()
        print("Environment validated")
    except Exception as exc:
        print(f"bootstrap error: {exc}")


if __name__ == "__main__":
    main()
