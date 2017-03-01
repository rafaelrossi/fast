/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_value_010317165442
#define KSERGEY_value_010317165442

namespace fast {

enum value_state_t
{
    state_undefined,
    state_assigned,
    state_empty
};

template< class T >
class value
{
private:
    T val_{};
    value_state_t state_{state_undefined};

public:
    value() = default;

    value_state_t state() const noexcept
    { return state_; }

    bool empty() const noexcept
    { return state_ == state_empty; }

    bool assigned() const noexcept
    { return state_ == state_assigned; }

    bool undefined() const noexcept
    { return state_ == state_undefined; }

    void set_state(value_state_t val) noexcept
    { state_ = val; }
};

} /* namespace fast */

#endif /* KSERGEY_value_010317165442 */
