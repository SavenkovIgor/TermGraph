import os
from shutil import which


class ToolsValidator:
    """Validate build tools availability."""

    def __init__(self) -> None:
        self._checks = [
            self.checkPython,
            self.checkGit,
            self.checkConan,
            self.checkCmake,
            self.checkNinja,
            self.checkClang,
            self.checkQt,
        ]

    def checkPython(self) -> None:
        if which("python3") is None:
            raise RuntimeError("python3 not found")

    def checkGit(self) -> None:
        if which("git") is None:
            raise RuntimeError("git not found")

    def checkConan(self) -> None:
        if which("conan") is None:
            raise RuntimeError("conan not found")

    def checkCmake(self) -> None:
        if which("cmake") is None:
            raise RuntimeError("cmake not found")

    def checkNinja(self) -> None:
        if which("ninja") is None:
            raise RuntimeError("ninja not found")

    def checkClang(self) -> None:
        if which("clang") is None:
            raise RuntimeError("clang not found")

    def checkQt(self) -> None:
        qt_root = os.environ.get("QT_ROOT")
        if not qt_root:
            raise RuntimeError("QT_ROOT environment variable is not set")
        if not os.path.exists(qt_root):
            raise RuntimeError(f"Qt root path does not exist: {qt_root}")

    def check_all(self) -> None:
        for check in self._checks:
            check()
