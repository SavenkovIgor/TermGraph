add_library(HelloModule STATIC)
set_target_properties(HelloModule PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_SCAN_FOR_MODULES TRUE
)

target_sources(HelloModule
    PUBLIC
        hello.h
    PRIVATE
        hello.cpp
        hello_impl.cppm
)

add_executable(ModuleExample main.cpp)
set_target_properties(ModuleExample PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_SCAN_FOR_MODULES TRUE
)

target_link_libraries(ModuleExample PRIVATE HelloModule)
