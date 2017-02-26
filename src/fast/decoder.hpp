/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_decoder_250217234950
#define KSERGEY_decoder_250217234950

namespace fast {

/** FAST stream decoder */
class decoder
{
private:
    const char* first_{nullptr};
    const char* last_{nullptr};

public:
    decoder(const decoder&) = delete;
    decoder& operator=(const decoder&) = delete;

    /** Construct decoder */
    decoder(const char* first, const char* last)
        : first_{first}
        , last_{last}
    {}
};

} /* namespace fast */

#endif /* KSERGEY_decoder_250217234950 */
