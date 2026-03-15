---
name: clang-management
description: 'Clang version management for TermGraph. Use when: bumping clang version, adding clang references, editing Dockerfile or CI clang setup, reviewing compiler version pins, modifying CMakePresets or Conan profiles, troubleshooting unversioned clang resolution.'
user-invocable: false
---

# Clang Version Management

## When to Use

- Bumping clang to a new version
- Adding or reviewing clang/clang++ references in any file
- Editing `.devcontainer/Dockerfile`, `.github/workflows/ci.yml`, or Conan profiles
- Reviewing PRs that touch compiler toolchain configuration

## Convention

The project uses **unversioned clang references** everywhere except three source-of-truth files. This minimizes the number of places to update when bumping the clang version.

## Version Pinning — Exactly 3 Files

| File | Variable | Purpose |
|------|----------|---------|
| `.devcontainer/Dockerfile` | `ARG CLANG_VERSION=XX` | DevContainer build |
| `.github/workflows/ci.yml` | `CLANG_VERSION: XX` env var | CI pipeline |
| `conanfiles/profile/base/clang` | `compiler.version=XX` | Conan ABI compatibility |

## Unversioned References — All Other Files

These files MUST use `clang` / `clang++` without version suffixes:

- `CMakePresets.json` — `clang` / `clang++` (resolved via PATH)
- `conanfiles/profile/base/clang` — `CC=clang` / `CXX=clang++` in `[buildenv]`
- `.vscode/settings.json`, `.vscode/c_cpp_properties.json` — `clang++`

## How Unversioned Names Resolve

- **DevContainer:** `ENV PATH="/usr/lib/llvm-XX/bin:${PATH}"` — LLVM tools available unversioned
- **CI:** `echo "/usr/lib/llvm-XX/bin" >> $GITHUB_PATH` — same mechanism via GitHub Actions
- **`cc` / `c++` system links:** Set via `update-alternatives` in both Dockerfile and CI (not in the LLVM bin directory)

## Procedure: Bumping Clang Version

1. Update `ARG CLANG_VERSION` in `.devcontainer/Dockerfile`
2. Update `CLANG_VERSION` env var in `.github/workflows/ci.yml`
3. Update `compiler.version` in `conanfiles/profile/base/clang`
4. Verify no version-suffixed clang references exist elsewhere:
   ```bash
   grep -rn 'clang-[0-9]\|clang++-[0-9]' --include='*.json' --include='*.cmake' --include='*.py' --include='*.md' .
   ```
5. Rebuild and test: `./project.py --build --preset desktop_dev`

## Review Checklist

- [ ] No version-suffixed clang names outside the 3 pinned files
- [ ] All 3 pinned files use the same version number
- [ ] PATH-based resolution still works (check Dockerfile `ENV PATH` and CI `GITHUB_PATH`)
- [ ] `update-alternatives` commands in Dockerfile and CI are consistent
