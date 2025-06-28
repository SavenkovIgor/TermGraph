# TermGraph Agent Guidelines

## Style
- **C++**: Use 4 spaces for indentation. Functions and variables use `snake_case`; classes use `CamelCase`.
- **Python**: Use 4 spaces for indentation. Follow `snake_case` for variables and functions, and `CamelCase` for classes.

## Testing
- Run `pytest -q` from the repository root for Python tests.
- Run C++ tests using `./project.py --test` or `ctest` in the build directory.

## Pull Requests
- Include a concise summary of changes.
- Mention test results in a **Testing** section.
- Branch names must be ASCII/English only.
