/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_pmap_decoder_010317223333
#define KSERGEY_pmap_decoder_010317223333

#include <cstdint>
#include <cassert>
#include "data_parser.hpp"

namespace fast {

/** FAST presence map decoder */
class pmap_decoder
{
private:
    std::uint64_t value_{0};
    std::uint64_t mask_{0};

public:
    /** Default constructor */
    pmap_decoder() = default;

    /** Load PMAP from parser */
    __force_inline void load(data_parser& parser)
    {
        /* Read pmap and pmap mask */
        parser.parse_pmap(value_, mask_);
        /* Move cursor to first bit */
        next();
    }

    /** Check is current bit is set */
    __force_inline bool is_bit_set() const noexcept
    {
        assert( mask_ > 0 );
        return (value_ & mask_);
    }

    /** Shift mask to next bit */
    __force_inline void next() noexcept
    { mask_ >>= 1; }

    /** Return current mask */
    __force_inline std::uint64_t mask() const noexcept
    { return mask_; }
};

} /* namespace fast */

#endif /* KSERGEY_pmap_decoder_010317223333 */
