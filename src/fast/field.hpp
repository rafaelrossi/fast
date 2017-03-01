/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_field_260217001137
#define KSERGEY_field_260217001137

#include <cstdint>
#include <string>
#include <experimental/optional>
#include "pmap.hpp"
#include "field_attrs.hpp"
#include "value.hpp"

namespace fast {

using std::experimental::optional;

/** Field class */
template< field_op_t Op, field_presence_t Presence >
class field
{
private:
    std::string name_;
    std::uint32_t id_;

public:
    /** Construct field */
    field(const char* name, std::uint32_t id)
        : name_{name}
        , id_{id}
    {}

    /** Return true if PMAP bit required */
    constexpr bool is_pmap_required() const
    { return pmap_bit_required< Op, Presence >; }

    /** Return true if field could be nullable */
    constexpr bool is_nullable() const
    { return nullable< Op, Presence >; }
};

template< class T, field_op_t Op, field_presence_t Presence >
class field_decoder_int
    : public field< Op, Presence >
{
private:
    static_assert( is_int_v< T >, "" );

    using base = field< Op, Presence >;

    value< T > reset_value_;
    value< T > prev_value_;
    value< T > value_;

public:
    using base::base;


};

} /* namespace fast */

#endif /* KSERGEY_field_260217001137 */
