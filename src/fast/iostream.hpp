/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_iostream_020317161810
#define KSERGEY_iostream_020317161810

#include <iostream>
#include <cmath>
#include "field.hpp"
#include "compiler.hpp"

namespace fast {

/** Serialize decimal */
__force_inline std::ostream& operator<<(std::ostream& os, const decimal& value)
{ return os << double(value.mantissa * std::pow(10.0, value.exponent)); }

/** Serialize unicode_string_cref */
__force_inline std::ostream& operator<<(std::ostream& os, const unicode_string_cref& value)
{ return os << ascii_string_cref{reinterpret_cast< const char* >(value.data()), value.size()}; }

/** Serialize vector_cref */
__force_inline std::ostream& operator<<(std::ostream& os, const vector_cref& value)
{ return os << ascii_string_cref{reinterpret_cast< const char* >(value.data()), value.size()}; }

/** Serialize field to stream */
template< class TypeT, field_op_t Op, field_presence_t Presence >
__force_inline std::ostream& operator<<(std::ostream& os, const field< TypeT, Op, Presence >& f)
{
    if (f.present()) {
        os << f.name() << "=" << f.value();
    } else {
        os << f.name() << "=NULL";
    }
    return os;
}

} /* namespace fast */

#endif /* KSERGEY_iostream_020317161810 */
