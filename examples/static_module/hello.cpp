module;

#include "hello.h"

import hello_internal;

std::string hello_world() {
    return hello_world_internal();
}
