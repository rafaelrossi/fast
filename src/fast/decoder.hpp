/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_decoder_250217234950
#define KSERGEY_decoder_250217234950

#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include "compiler.hpp"

namespace fast {

template< class T >
constexpr bool is_fast_signed_int_v =
    std::is_same< T, std::int32_t >::value || std::is_same< T, std::int64_t >::value;

template< class T >
constexpr bool is_fast_unsigned_int_v =
    std::is_same< T, std::uint32_t >::value || std::is_same< T, std::uint64_t >::value;

template< class T >
constexpr bool is_fast_int_v = is_fast_signed_int_v< T > || is_fast_unsigned_int_v< T >;

/** FAST stream decoder */
class decoder
{
private:
    char* first_{nullptr};
    char* last_{nullptr};

public:
    decoder(const decoder&) = delete;
    decoder& operator=(const decoder&) = delete;

    /** Construct decoder */
    decoder(char* first, char* last)
        : first_{first}
        , last_{last}
    {}

    /** Decode integer non-nullable type */
    template< class IntT >
    __force_inline std::enable_if_t< is_fast_int_v< IntT > > decode(IntT& result)
    {
        result = 0;
        while (__likely(first_ != last_)) {
            auto c = *first_++;
            result = (result << 7) | (c & 0x7f);
            if (c & 0x80) {
                return;
            }
        }

        throw std::runtime_error("Integer field decoding error");
    }

    /**
     * Decode signed integer nullable type
     * @return false if null-value decoded
     */
    template< class IntT >
    __force_inline std::enable_if_t< is_fast_signed_int_v< IntT >, bool > decode_nullable(IntT& result)
    {
        decode(result);
        if (result == 0) {
            return false;
        }
        if (result > 0) {
            --result;
        }
        return true;
    }

    /**
     * Decode unsigned integer nullable type
     * @return false if null-value decoded
     */
    template< class IntT >
    __force_inline std::enable_if_t< is_fast_unsigned_int_v< IntT >, bool > decode_nullable(IntT& result)
    {
        decode(result);
        if (result == 0) {
            return false;
        }
        --result;
        return true;
    }
};

} /* namespace fast */

#endif /* KSERGEY_decoder_250217234950 */
