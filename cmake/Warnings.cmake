# Copyright © 2016-2025. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(project_warnings INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(project_warnings INTERFACE
        -Werror
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wcast-qual
        -Wformat=2
        -Wundef
        -Wcast-align
        -Wunused
        -Wdouble-promotion
        -Wimplicit-fallthrough
        -Wextra-semi
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wold-style-cast
    )

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Suppress GNU extension warning: ##__VA_ARGS__ is a GNU extension,
        # used legitimately in Qt macros. Emscripten (Clang-based) also needs this.
        target_compile_options(project_warnings INTERFACE
            -Wno-gnu-zero-variadic-macro-arguments
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(project_warnings INTERFACE
            -Wno-variadic-macros
        )
    endif()
endif()
