# Conan custom deployer: https://docs.conan.io/2/reference/extensions/deployers.html
#
# Deployers run once per `conan install`, right after dependency resolution and
# right before generators (CMakeDeps/CMakeToolchain), and can copy/link files
# out of the Conan cache into a folder the caller controls (--deployer-folder).
#
# Conan invokes them by filename: `--deployer=conanfiles/deployers/tool_requires_deploy.py`
# calling this module's `deploy(graph, output_folder, **kwargs)` function.
# `graph` is the resolved dependency graph, `output_folder` is whatever path
# was passed via `--deployer-folder` (or -of if that flag is omitted).
# `**kwargs` is required by Conan's calling convention even if unused, since
# future Conan versions may pass extra arguments here.
from pathlib import Path
from typing import Any


def deploy(graph: Any, output_folder: str, **kwargs: Any) -> None:
    """Symlink binaries of build-context tool_requires (cmake, ninja, ...) into a stable bin/ folder."""
    bin_folder = Path(output_folder) / "bin"
    bin_folder.mkdir(parents=True, exist_ok=True)

    # `graph.root.conanfile` is our own conanfile.py at the root of the dependency graph.
    # `.dependencies.build` holds only the *build context* dependencies, i.e. the ones
    # declared via `tool_requires()` (cmake, ninja) - not `requires()` libraries.
    # Each `dep` here is a ConanFileInterface, a read-only view of the dependency's
    # conanfile: `dep.package_folder` points into the Conan cache (a path containing
    # a package-id hash that changes whenever the package is rebuilt/updated).
    conanfile = graph.root.conanfile
    for _, dep in conanfile.dependencies.build.items():
        # `cpp_info.bindirs` lists the folders (relative to package_folder) where
        # the package's executables live; defaults to ["bin"] if unset.
        for bindir in dep.cpp_info.bindirs or ["bin"]:
            src_dir = Path(dep.package_folder) / bindir
            if not src_dir.is_dir():
                continue
            for src_file in src_dir.iterdir():
                # Re-link on every install: the Conan cache path (src_dir) changes
                # across package rebuilds, but this stable bin_folder never does.
                link = bin_folder / src_file.name
                if link.is_symlink() or link.exists():
                    link.unlink()
                link.symlink_to(src_file)
