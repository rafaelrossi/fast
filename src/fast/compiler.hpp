/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_compiler_220217125612
#define KSERGEY_compiler_220217125612

#ifndef __force_inline
#   define __force_inline inline __attribute__((always_inline))
#endif

#ifndef __no_inline
#   define __no_inline __attribute__((noinline))
#endif

#ifndef __packed
#   define __packed __attribute__((packed))
#endif

#ifndef __unused
#   define __unused __attribute__((unused))
#endif

#ifndef __likely
#   define __likely(x) __builtin_expect(static_cast< bool >(x), true)
#endif

#ifndef __unlikely
#   define __unlikely(x) __builtin_expect(static_cast< bool >(x), false)
#endif

#endif /* KSERGEY_compiler_220217125612 */
