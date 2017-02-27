/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include "fast/schema.hpp"
#include "fast/decoder.hpp"
#include "fast/pmap.hpp"
#include "fast/field.hpp"

std::vector< char > load_packet(const char* path);

int main(int argc, char* argv[])
{
    try {
        /* Load schema */
        fast::schema schema{"FIX50SP2-2017-Mar.xml"};

        /* Load sample packet */
        auto packet = load_packet("packet.data");

        /* Create decoder */
        fast::decoder decoder{packet.data(), packet.data() + packet.size()};

        /* Create PMAP and load value */
        fast::pmap pmap;

        /* Decode PMAP */
        pmap.load(decoder);

        /* Check if PMAP bit set */
        if (pmap.is_bit_set()) {
            /* Decode template ID */
            std::uint32_t template_id;
            decoder.decode_int(template_id);
            std::cout << "Packet template id " << template_id << '\n';
            pmap.next();
        }

        std::cout << fast::pmap_bit_required< fast::op_none, fast::presence_mandatory > << ' '
            << fast::pmap_bit_required< fast::op_none, fast::presence_optional > << '\n';
        std::cout << fast::pmap_bit_required< fast::op_constant, fast::presence_mandatory > << ' '
            << fast::pmap_bit_required< fast::op_constant, fast::presence_optional > << '\n';
        std::cout << fast::pmap_bit_required< fast::op_copy, fast::presence_mandatory > << ' '
            << fast::pmap_bit_required< fast::op_copy, fast::presence_optional > << '\n';
        std::cout << fast::pmap_bit_required< fast::op_default, fast::presence_mandatory > << ' '
            << fast::pmap_bit_required< fast::op_default, fast::presence_optional > << '\n';
        std::cout << fast::pmap_bit_required< fast::op_delta, fast::presence_mandatory > << ' '
            << fast::pmap_bit_required< fast::op_delta, fast::presence_optional > << '\n';
        std::cout << fast::pmap_bit_required< fast::op_increment, fast::presence_mandatory > << ' '
            << fast::pmap_bit_required< fast::op_increment, fast::presence_optional > << '\n';
        std::cout << fast::pmap_bit_required< fast::op_tail, fast::presence_mandatory > << ' '
            << fast::pmap_bit_required< fast::op_tail, fast::presence_optional > << '\n';

        /* Construct field */
        fast::field< fast::op_constant, fast::presence_mandatory > field1{"MessageType", 35};

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

std::vector< char > load_packet(const char* path)
{
    using namespace std::string_literals;

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector< char > result;
    result.resize(size);

    if (!file.read(result.data(), size)) {
        throw std::runtime_error("Failed to read packet file "s + path);
    }

    return std::move(result);
}
