#!/usr/bin/env python3

from env_validator import ToolsValidator


def main() -> None:
    ToolsValidator().check_all()
    print("Environment validated")


if __name__ == "__main__":
    main()
