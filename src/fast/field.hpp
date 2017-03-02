/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_field_260217001137
#define KSERGEY_field_260217001137

#include <cstdint>
#include <string>
#include "pmap_decoder.hpp"
#include "field_detail.hpp"
#include "value_storage.hpp"

namespace fast {

template< class T >
struct field_context
{
    value_storage< T > reset_value;
    value_storage< T > value;
};

template< field_op_t Op, field_presence_t Presence >
struct field_decoder;

template<>
struct field_decoder< op_none, presence_mandatory >
{
    static_assert( pmap_bit_required< op_none, presence_mandatory > == false, "" );

    template< class T >
    __force_inline static void decode(data_parser& parser, pmap_decoder& pmap, field_context< T >& ctx)
    {
        parser.parse(ctx.value.get());
        ctx.value.set_assigned();
    }
};

template<>
struct field_decoder< op_none, presence_optional >
{
    static_assert( pmap_bit_required< op_none, presence_optional > == false, "" );

    template< class T >
    __force_inline static void decode(data_parser& parser, pmap_decoder& pmap, field_context< T >& ctx)
    {
        if (parser.parse_nullable(ctx.value.get())) {
            ctx.value.set_assigned();
        } else {
            ctx.value.set_empty();
        }
    }
};

template<>
struct field_decoder< op_copy, presence_mandatory >
{
    static_assert( pmap_bit_required< op_copy, presence_mandatory > == true, "" );

    template< class T >
    __force_inline static void decode(data_parser& parser, pmap_decoder& pmap, field_context< T >& ctx)
    {
        if (pmap.is_bit_set()) {
            parser.parse(ctx.value.get());
            ctx.value.set_assigned();
        } else {
            if (ctx.value.undefined()) {
                assert( ctx.reset_value.assigned() );
                ctx.value = ctx.reset_value;
            } else {
                assert( ctx.value.assigned() );
            }
        }
        pmap.next();
    }
};

template<>
struct field_decoder< op_copy, presence_optional >
{
    static_assert( pmap_bit_required< op_copy, presence_optional > == true, "" );

    template< class T >
    __force_inline static void decode(data_parser& parser, pmap_decoder& pmap, field_context< T >& ctx)
    {
        if (pmap.is_bit_set()) {
            if (parser.parse_nullable(ctx.value.get())) {
                ctx.value.set_assigned();
            } else {
                ctx.value.set_empty();
            }
        } else {
            if (ctx.value.undefined()) {
                if (__likely(ctx.reset_value.assigned())) {
                    ctx.value = ctx.reset_value;
                } else {
                    ctx.value.set_empty();
                }
            } else {
                assert( ctx.value.assigned() || ctx.value.empty() );
            }
        }
        pmap.next();
    }
};

template<>
struct field_decoder< op_increment, presence_mandatory >
{
    static_assert( pmap_bit_required< op_increment, presence_mandatory > == true, "" );

    template< class T >
    __force_inline static void decode(data_parser& parser, pmap_decoder& pmap, field_context< T >& ctx)
    {
        static_assert( is_int_v< T >, "" );

        if (pmap.is_bit_set()) {
            parser.parse(ctx.value.get());
            ctx.value.set_assigned();
        } else {
            if (ctx.value.undefined()) {
                assert( ctx.reset_value.assigned() );
                ctx.value = ctx.reset_value;
            } else if (ctx.value.assigned()) {
                ++ctx.value.get();
            } else {
                assert( false );
            }
        }
        pmap.next();
    }
};

template<>
struct field_decoder< op_increment, presence_optional >
{
    static_assert( pmap_bit_required< op_increment, presence_optional > == true, "" );

    template< class T >
    __force_inline static void decode(data_parser& parser, pmap_decoder& pmap, field_context< T >& ctx)
    {
        static_assert( is_int_v< T >, "" );

        if (pmap.is_bit_set()) {
            if (parser.parse_nullable(ctx.value.get())) {
                ctx.value.set_assigned();
            } else {
                ctx.value.set_empty();
            }
        } else {
            if (ctx.value.undefined()) {
                if (__likely(ctx.reset_value.assigned())) {
                    ctx.value = ctx.reset_value;
                } else {
                    ctx.value.set_empty();
                }
            } else if (ctx.value.assigned()) {
                ++ctx.value.get();
            }
        }
        pmap.next();
    }
};

template<>
struct field_decoder< op_delta, presence_mandatory >
{
    static_assert( pmap_bit_required< op_delta, presence_mandatory > == false, "" );
};

template<>
struct field_decoder< op_delta, presence_optional >
{
    static_assert( pmap_bit_required< op_delta, presence_optional > == false, "" );
};

template<>
struct field_decoder< op_default, presence_mandatory >
{
    static_assert( pmap_bit_required< op_default, presence_mandatory > == true, "" );

    template< class T >
    __force_inline static void decode(data_parser& parser, pmap_decoder& pmap, field_context< T >& ctx)
    {
        if (__unlikely(pmap.is_bit_set())) {
            parser.parse(ctx.value.get());
            ctx.value.set_assigned();
        } else {
            assert( ctx.reset_value.assigned() );
            ctx.value = ctx.reset_value;
        }
        pmap.next();
    }
};

template<>
struct field_decoder< op_default, presence_optional >
{
    static_assert( pmap_bit_required< op_default, presence_optional > == true, "" );

    template< class T >
    __force_inline static void decode(data_parser& parser, pmap_decoder& pmap, field_context< T >& ctx)
    {
        if (pmap.is_bit_set()) {
            if (parser.parse_nullable(ctx.value.get())) {
                ctx.value.set_assigned();
            } else {
                ctx.value.set_empty();
            }
        } else {
            if (__likely(ctx.reset_value.assigned())) {
                ctx.value = ctx.reset_value;
            } else {
                ctx.value.set_empty();
            }
        }
        pmap.next();
    }
};

template<>
struct field_decoder< op_constant, presence_mandatory >
{
    static_assert( pmap_bit_required< op_constant, presence_mandatory > == false, "" );

    template< class T >
    __force_inline static void decode(data_parser& parser, pmap_decoder& pmap, field_context< T >& ctx)
    {
        assert( ctx.reset_value.assigned() );

        if (__unlikely(!ctx.value.assigned())) {
            ctx.value = ctx.reset_value;
        }
    }
};

template<>
struct field_decoder< op_constant, presence_optional >
{
    static_assert( pmap_bit_required< op_constant, presence_optional > == true, "" );

    template< class T >
    __force_inline static void decode(data_parser& parser, pmap_decoder& pmap, field_context< T >& ctx)
    {
        assert( ctx.reset_value.assigned() );

        if (pmap.is_bit_set()) {
            ctx.value = ctx.reset_value;
        } else {
            ctx.value.set_empty();
        }

        pmap.next();
    }
};

/** Field class */
template< class TypeT, field_op_t Op, field_presence_t Presence >
class field
{
private:
    std::string name_;
    std::uint32_t id_;

    friend class field_decoder< Op, Presence >;

    field_context< TypeT > context_;

public:
    /** Construct field */
    field(const char* name, std::uint32_t id)
        : name_{name}
        , id_{id}
    {}

    /** Construct field */
    field(const char* name, std::uint32_t id, TypeT reset_value)
        : name_{name}
        , id_{id}
    {
        context_.reset_value.get() = reset_value;
        context_.reset_value.set_assigned();
    }

    /** Return field name */
    __force_inline const std::string& name() const noexcept
    { return name_; }

    /** Return field id */
    __force_inline std::uint32_t id() const noexcept
    { return id_; }

    /** Return value of field */
    __force_inline TypeT value() const noexcept
    { return context_.value.get(); }

    /** Return true if value of field is present */
    __force_inline bool present() const noexcept
    { return context_.value.assigned(); }

    /** Decode field data */
    __force_inline void decode(data_parser& parser, pmap_decoder& pmap)
    { field_decoder< Op, Presence >::decode(parser, pmap, context_); }
};

} /* namespace fast */

#endif /* KSERGEY_field_260217001137 */
