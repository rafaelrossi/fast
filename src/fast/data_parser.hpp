/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_data_parser_010317134652
#define KSERGEY_data_parser_010317134652

#include <stdexcept>
#include "compiler.hpp"
#include "types.hpp"

namespace fast {

/** FAST data parser */
class data_parser
{
private:
    char* first_{nullptr};
    char* last_{nullptr};

public:
    data_parser(const data_parser&) = delete;
    data_parser& operator=(const data_parser&) = delete;

    /** Default constructor */
    data_parser() = default;

    /** Constuct data_parser */
    data_parser(char* first, char* last)
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
        parse(result);
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


    /** Parse decimal */
    __force_inline void parse(decimal& result)
    {
        parse(result.exponent);
        if (__unlikely(result.exponent > 63 || result.exponent < -63)) {
            throw std::runtime_error("Decimal exponent out of range [-63; 63]");
        }
        parse(result.mantissa);
    }

    /** Parse decimal (nullable) */
    __force_inline bool parse_nullable(decimal& result)
    {
        if (!parse_nullable(result.exponent)) {
            return false;
        }
        if (__unlikely(result.exponent > 63 || result.exponent < -63)) {
            throw std::runtime_error("Decimal exponent out of range [-63; 63]");
        }
        parse(result.mantissa);
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
            return true;
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
            return true;
        } else {
            return false;
        }
    }
};

} /* namespace fast */

#endif /* KSERGEY_data_parser_010317134652 */
