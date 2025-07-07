module;

#include <string>

export module hello_internal;

export std::string hello_world_internal() {
    return "Hello, World!";
}
