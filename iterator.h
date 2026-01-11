#ifndef __ITERATOR__
#define __ITERATOR__

namespace nig {
    template <typename C, typename T>
    class iterator_base {
        static_assert(sizeof(T) == 0);
    };

    template <typename C, typename T>
    class reverse_iterator_base {
        static_assert(sizeof(T) == 0);
    };
}  // namespace nig
#endif