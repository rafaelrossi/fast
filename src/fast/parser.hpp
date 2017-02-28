/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_parser_280217225024
#define KSERGEY_parser_280217225024

#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <experimental/string_view>
#include "compiler.hpp"

namespace fast {

/** Check type T is signed integer (int32 or int64) */
template< class T >
constexpr bool is_signed_int_v =
    std::is_same< T, std::int32_t >::value || std::is_same< T, std::int64_t >::value;

/** Check type T is unsigned integer (uint32 or uint64) */
template< class T >
constexpr bool is_unsigned_int_v =
    std::is_same< T, std::uint32_t >::value || std::is_same< T, std::uint64_t >::value;

/** Check type T integer (int32, int64, uint32 or uint64) */
template< class T >
constexpr bool is_int_v = is_signed_int_v< T > || is_unsigned_int_v< T >;

/** Make unicode char unique */
struct unicode_char
{
    char value;
};

using ascii_string_cref = std::experimental::basic_string_view< char >;
using unicode_string_cref = std::experimental::basic_string_view< unicode_char >;
using vector_cref = std::experimental::basic_string_view< std::int8_t >;

static_assert( sizeof(unicode_char) == sizeof(char), "" );
static_assert( std::is_same< std::int8_t, char >::value == false, "" );

/** FAST data parser */
class parser
{
private:
    char* first_{nullptr};
    char* last_{nullptr};

public:
    parser(const parser&) = delete;
    parser& operator=(const parser&) = delete;

    /** Default constructor */
    parser() = default;

    /** Constuct parser */
    parser(char* first, char* last)
        : first_{first}
        , last_{last}
    {}

    /** Parse PMAP */
    __force_inline void parse_pmap(std::uint64_t& pmap, std::uint64_t& pmap_mask)
    {
        pmap = 0;
        pmap_mask = 1;

        while (__likely(first_ != last_)) {
            auto c = *first_++;
            pmap = (pmap << 7) | (c & 0x7f);
            pmap_mask <<= 7;
            if (c & 0x80) {
                return;
            }
            /* TODO: check overflow */
        }
        throw std::runtime_error("Unexpected end of data (PMAP parsing)");
    }

    /** Parse signed/unsigned integer */
    template< class IntT >
    __force_inline std::enable_if_t< is_int_v< IntT > > parse(IntT& result)
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
        throw std::runtime_error("Unexpected end of data (integer parsing)");
    }

    /**
     * Parse signed integer (nullable)
     * @return false if null-value parsed
     */
    template< class IntT >
    __force_inline std::enable_if_t< is_signed_int_v< IntT >, bool > parse_nullable(IntT& result)
    {
        /* Parse raw value */
        parse_int(result);
        if (result == 0) {
            /* Parsed null value */
            return false;
        }
        /* Parsed non-null value */
        if (result > 0) {
            --result;
        }
        return true;
    }

    /**
     * Parse unsigned integer (nullable)
     * @return false if null-value decoded
     */
    template< class IntT >
    __force_inline std::enable_if_t< is_unsigned_int_v< IntT >, bool > parse_nullable(IntT& result)
    {
        /* Parse raw value */
        parse(result);
        if (result == 0) {
            /* Parsed null value */
            return false;
        }
        /* Parsed non-null value */
        --result;
        return true;
    }

    /** Parse ascii string */
    __force_inline void parse(ascii_string_cref& result)
    {
        char* data = first_;
        while (__likely(first_ != last_)) {
            if (*first_ & 0x80) {
                *first_ &= 0x7F;
                ++first_;
                result = ascii_string_cref{data, std::size_t(first_ - data)};
                return;
            } else {
                ++first_;
            }
        }

        throw std::runtime_error("Unexpected end of data (ascii string parsing)");
    }

    /** Parse ascii string (nullable) */
    __force_inline bool parse_nullable(ascii_string_cref& result)
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
                    result = ascii_string_cref{};
                } else {
                    result = ascii_string_cref{data, std::size_t(first_ - data)};
                }
                return true;
            } else {
                ++first_;
            }
        }

        throw std::runtime_error("Unexpected end of data (ascii string parsing)");
    }

    /** Parse unicode string */
    __force_inline void parse(unicode_string_cref& result)
    {
        std::uint32_t len;
        parse(len);
        if (__unlikely(first_ + len > last_)) {
            throw std::runtime_error("Unexpected end of data (unicode string parsing)");
        }
        result = unicode_string_cref{reinterpret_cast< unicode_char* >(first_), len};
        first_ += len;
    }

    /** Parse unicode string (nullable) */
    __force_inline bool parse_nullable(unicode_string_cref& result)
    {
        std::uint32_t len;
        if (parse_nullable(len)) {
            if (__unlikely(first_ + len > last_)) {
                throw std::runtime_error("Unexpected end of data (unicode string parsing)");
            }
            result = unicode_string_cref{reinterpret_cast< unicode_char* >(first_), len};
            first_ += len;
        } else {
            return false;
        }
    }

    /** Parse vector of ints */
    __force_inline void parse(vector_cref& result)
    {
        std::uint32_t len;
        parse(len);
        if (__unlikely(first_ + len > last_)) {
            throw std::runtime_error("Unexpected end of data (vector parsing)");
        }
        result = vector_cref{reinterpret_cast< std::int8_t* >(first_), len};
        first_ += len;
    }

    /** Parse vector of ints (nullable) */
    __force_inline bool parse_nullable(vector_cref& result)
    {
        std::uint32_t len;
        if (parse_nullable(len)) {
            if (__unlikely(first_ + len > last_)) {
                throw std::runtime_error("Unexpected end of data (vector parsing)");
            }
            result = vector_cref{reinterpret_cast< std::int8_t* >(first_), len};
            first_ += len;
        } else {
            return false;
        }
    }
};

} /* namespace fast */

#endif /* KSERGEY_parser_280217225024 */
