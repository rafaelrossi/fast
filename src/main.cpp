/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <stdexcept>
#include "fast/schema.hpp"
#include "fast/decoder.hpp"

int main(int argc, char* argv[])
{
    try {
        fast::schema schema{"FIX50SP2-2017-Mar.xml"};

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
