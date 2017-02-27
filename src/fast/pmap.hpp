/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_pmap_220217154418
#define KSERGEY_pmap_220217154418

#include <cstdint>
#include <cassert>
#include "decoder.hpp"

namespace fast {

class pmap
{
private:
    std::uint64_t value_{0};
    std::uint64_t mask_{0};

public:
    pmap() = default;

    pmap(std::uint64_t value, std::uint64_t mask)
        : value_{value}
        , mask_{mask}
    {}

    /** Load PMAP from decoder */
    __force_inline void load(decoder& d)
    {
        /* Load from decoder */
        d.decode_pmap(value_, mask_);
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

#endif /* KSERGEY_pmap_220217154418 */
