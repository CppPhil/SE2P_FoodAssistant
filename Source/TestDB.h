#pragma once
#include "GlobalHeader.h"

namespace testCrap {
    void testDB();
    int giveOne();

    struct Person final {
        fa::String name;
        fa::String address;
        int age;
    };
}
