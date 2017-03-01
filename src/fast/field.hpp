/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_field_260217001137
#define KSERGEY_field_260217001137

#include <cstdint>
#include <string>
#include "pmap_decoder.hpp"
#include "field_detail.hpp"
#include "value.hpp"

namespace fast {

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

template< class T >
struct field_context
{
    value< T > reset_value_;
    value< T > prev_value_;
    value< T > value_;
};

template< field_op_t Op, field_presence_t Presence >
struct field_decoder;

template<>
struct field_decoder< op_none, presence_mandatory >
{
//template< class T >
//__force_inline static void decode(data_parser& parser, pmap& map)

};

} /* namespace fast */

#endif /* KSERGEY_field_260217001137 */
