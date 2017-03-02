/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_types_020317161659
#define KSERGEY_types_020317161659

#include <cstdint>
#include <type_traits>
#include <experimental/string_view>

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

/** Decimal type */
struct decimal
{
    std::int64_t mantissa;
    std::int32_t exponent;
};

using ascii_string_cref = std::experimental::basic_string_view< char >;
using unicode_string_cref = std::experimental::basic_string_view< unicode_char >;
using vector_cref = std::experimental::basic_string_view< std::int8_t >;

static_assert( sizeof(unicode_char) == sizeof(char), "" );
static_assert( std::is_same< std::int8_t, char >::value == false, "" );

} /* namespace fast */

#endif /* KSERGEY_types_020317161659 */
