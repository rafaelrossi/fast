/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_field_attrs_010317133102
#define KSERGEY_field_attrs_010317133102

namespace fast {

/** Field operators attribute */
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

/** Field presence attribute */
enum field_presence_t
{
    presence_mandatory,
    presence_optional
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

} /* namespace fast */

#endif /* KSERGEY_field_attrs_010317133102 */
