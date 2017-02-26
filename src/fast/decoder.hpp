/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_decoder_250217234950
#define KSERGEY_decoder_250217234950

#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <experimental/string_view>
#include "compiler.hpp"

namespace fast {

using std::experimental::string_view;
using byte_view = std::experimental::basic_string_view< std::int8_t >;

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

    /* TODO: add skip field by type */

    /** Decode integer non-nullable type */
    template< class IntT >
    __force_inline std::enable_if_t< is_fast_int_v< IntT > > decode_int(IntT& result)
    {
        result = 0;
        while (__likely(first_ != last_)) {
            auto c = *first_++;
            result = (result << 7) | (c & 0x7f);
            if (c & 0x80) {
                return;
            }
            /* TODO: check overflow */
        }

        throw std::runtime_error("Unexpected end of data (integer field decoding)");
    }

    /**
     * Decode signed integer nullable type
     * @return false if null-value decoded
     */
    template< class IntT >
    __force_inline std::enable_if_t< is_fast_signed_int_v< IntT >, bool > decode_nullable_int(IntT& result)
    {
        decode_int(result);
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
    __force_inline std::enable_if_t< is_fast_unsigned_int_v< IntT >, bool > decode_nullable_int(IntT& result)
    {
        decode_int(result);
        if (result == 0) {
            return false;
        }
        --result;
        return true;
    }

    /**
     * Decode ascii string
     */
    __force_inline void decode_ascii(string_view& result)
    {
        char* data = first_;
        while (__likely(first_ != last_)) {
            if (*first_ & 0x80) {
                *first_ &= 0x7F;
                ++first_;
                result = string_view{data, std::size_t(first_ - data)};
                return;
            } else {
                ++first_;
            }
        }

        throw std::runtime_error("Unexpected end of data (ascii string field decoding)");
    }

    /**
     * Decode ascii nullable string
     */
    __force_inline bool decode_nullable_ascii(string_view& result)
    {
        char* data = first_;
        while (__likely(first_ != last_)) {
            if (*first_ & 0x80) {
                if (data == first_) {
                    /* null value found */
                    return false;
                }

                *first_ &= 0x7F;
                ++first_;

                if (__unlikely(*data == 0x00)) {
                    /* Empty string */
                    result = string_view{};
                } else {
                    result = string_view{data, std::size_t(first_ - data)};
                }
                return true;
            } else {
                ++first_;
            }
        }

        throw std::runtime_error("Unexpected end of data (ascii string field decoding)");
    }

    /** Decode unicode string */
    __force_inline void decode_unicode(string_view& result)
    {
        std::uint32_t len;
        decode_int(len);
        if (__unlikely(first_ + len > last_)) {
            throw std::runtime_error("Unexpected end of data (unicode string field decoding)");
        }
        result = string_view{first_, len};
        first_ += len;
    }

    /** Decode nullable unicode string */
    __force_inline bool decode_nullable_unicode(string_view& result)
    {
        std::uint32_t len;
        if (decode_nullable_int(len)) {
            if (__unlikely(first_ + len > last_)) {
                throw std::runtime_error("Unexpected end of data (unicode string field decoding)");
            }
            result = string_view{first_, len};
            first_ += len;
        } else {
            return false;
        }
    }

    /** Decode byte vector */
    __force_inline void decode_bytes(byte_view& result)
    {
        std::uint32_t len;
        decode_int(len);
        if (__unlikely(first_ + len > last_)) {
            throw std::runtime_error("Unexpected end of data (byte vector field decoding)");
        }
        result = byte_view{reinterpret_cast< std::int8_t* >(first_), len};
        first_ += len;
    }

    /** Decode nullable byte vector */
    __force_inline bool decode_nullable_bytes(byte_view& result)
    {
        std::uint32_t len;
        if (decode_nullable_int(len)) {
            if (__unlikely(first_ + len > last_)) {
                throw std::runtime_error("Unexpected end of data (byte vector field decoding)");
            }
            result = byte_view{reinterpret_cast< std::int8_t* >(first_), len};
            first_ += len;
        } else {
            return false;
        }
    }
};

} /* namespace fast */

#endif /* KSERGEY_decoder_250217234950 */
