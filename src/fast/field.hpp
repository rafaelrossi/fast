/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_field_260217001137
#define KSERGEY_field_260217001137

#include <cstdint>

namespace fast {

/*
 * PMAP bit required - yes/no
 */

enum class field_op
{
    none,
    constant,
    copy,
    default_,
    delta,
    increment,
    tail
};

enum class field_presence
{
    mandatory,
    optional
};

/*
 * PMAP Bit Requirement
 * ----------------------------------------
 *  Op          | mandatory     | optional
 * ----------------------------------------
 *  none        | no            | no
 *  constant    | no            | required
 *  copy        | required      | required
 *  default_    | required      | required
 *  delta       | no            | no
 *  increment   | required      | required
 *  tail        | required      | required
 * ----------------------------------------
 */

/*
 * Field Nullability
 * ----------------------------------------
 *  Op          | mandatory     | optional
 * ----------------------------------------
 *  none        | no            | yes
 *  constant    | no            | no
 *  copy        | no            | yes
 *  default_    | no            | yes
 *  delta       | no            | yes
 *  increment   | no            | yes
 *  tail        | no            | yes
 * ----------------------------------------
 */

class pmap;

template< std::uint32_t ID, class TypeT >
class field
{
private:
public:
    bool decode(const char*& encodedBegin, const char* encodedEnd,
            const pmap& map)
    {
        return false;
    }

};

} /* namespace fast */

#endif /* KSERGEY_field_260217001137 */
