/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_value_storage_010317224500
#define KSERGEY_value_storage_010317224500

namespace fast {

enum value_state_t
{
    state_undefined,
    state_assigned,
    state_empty
};

template< class T >
class value_storage
{
private:
    T val_{};
    value_state_t state_{state_undefined};

public:
    value_storage() = default;

    value_state_t state() const noexcept
    { return state_; }

    bool undefined() const noexcept
    { return state_ == state_undefined; }

    bool assigned() const noexcept
    { return state_ == state_assigned; }

    bool empty() const noexcept
    { return state_ == state_empty; }

    void set_state(value_state_t val) noexcept
    { state_ = val; }

    void set_undefined() noexcept
    { set_state(state_undefined); }

    void set_assigned() noexcept
    { set_state(state_assigned); }

    void set_empty() noexcept
    { set_state(state_empty); }

    T get() const noexcept
    { return val_; }

    T& get() noexcept
    { return val_; }
};

} /* namespace fast */

#endif /* KSERGEY_value_storage_010317224500 */
