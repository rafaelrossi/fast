/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_field_260217001137
#define KSERGEY_field_260217001137

#include <cstdint>
#include <string>
#include <experimental/optional>

namespace fast {

enum field_op_t
{
    op_none,
    op_constant,
    op_copy,
    op_default,
    op_delta,
    op_increment,
    op_tail
};

enum field_presence_t
{
    presence_mandatory,
    presence_optional
};

enum field_state_t
{
    state_undefined,
    state_assigned,
    state_empty
};

/*
 * PMAP Bit Requirement
 * ----------------------------------------
 *  Op          | mandatory     | optional
 * ----------------------------------------
 *  none        | no            | no
 *  constant    | no            | required
 *  copy        | required      | required
 *  default     | required      | required
 *  delta       | no            | no
 *  increment   | required      | required
 *  tail        | required      | required
 * ----------------------------------------
 */

template< field_op_t Op, field_presence_t Presence >
constexpr bool pmap_bit_required = true;

template< field_presence_t Presence >
constexpr bool pmap_bit_required< op_none, Presence > = false;

template< field_presence_t Presence >
constexpr bool pmap_bit_required< op_delta, Presence > = false;

template<>
constexpr bool pmap_bit_required< op_constant, presence_mandatory > = false;

/*
 * Field Nullability
 * ----------------------------------------
 *  Op          | mandatory     | optional
 * ----------------------------------------
 *  none        | no            | yes
 *  constant    | no            | no
 *  copy        | no            | yes
 *  default     | no            | yes
 *  delta       | no            | yes
 *  increment   | no            | yes
 *  tail        | no            | yes
 * ----------------------------------------
 */

template< field_op_t Op, field_presence_t Presence >
constexpr bool nullable = true;

template< field_op_t Op >
constexpr bool nullable< Op, presence_mandatory > = false;

template<>
constexpr bool nullable< op_constant, presence_optional > = false;

/** Field class */
template< field_op_t Op, field_presence_t Presence >
class field
{
private:
    std::string name_;
    std::uint32_t id_;

    field_state_t state_;
    field_state_t prev_state_;

    /* Type value_; */
    /* Type reset_value_; */
    /* Type prev_value_; */

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

} /* namespace fast */

#endif /* KSERGEY_field_260217001137 */
