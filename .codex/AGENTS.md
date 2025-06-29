# TermGraph Agent Guidelines

## Style
- **C++**: Use 4 spaces for indentation. Functions and variables use `snake_case`; classes use `CamelCase`.
- **Python**: Use 4 spaces for indentation. Follow `snake_case` for variables and functions, and `CamelCase` for classes.

## Build
- Build the project in the `desktop_dev` configuration using:
  `python3 project.py --build --preset desktop_dev`

## Testing
- Run `pytest -q` from the repository root for Python tests.
- Run C++ tests with `python3 project.py --test --preset desktop_dev`
  or run `ctest` in `build/desktop_dev`.

## Pull Requests
- Include a concise summary of changes.
- Mention test results in a **Testing** section.
- Branch names must be ASCII/English only.
