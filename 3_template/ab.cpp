#include <new>
#include "ab.h"

// Implementation


template <typename T>
class AB<T>::AB_impl {
public:
    AB_impl(T a, T b) : a{a}, b{b}
    {  }

    T get_a() const { return a; }
    T get_b() const { return b; }
    void inc_a_b()    { a+=1; b+=1; }
   
private:
    T a;
    T b;
};


// Interface
template <typename T>
AB<T>::AB(T a, T b)
{
    static_assert(storage_.Len   <= sizeof(           AB_impl),        "Too large!=>  1st template argument (Len1)   of Aligned_storer<Len1, Align1> storage_;");
    static_assert(storage_.Len   >= sizeof(           AB_impl),        "Too small!=>  1st template argument (Len1)   of Aligned_storer<Len1, Align1> storage_;");
    static_assert(storage_.Len   == sizeof(           AB_impl),        "Please adjust 1st template argument (Len1)   of Aligned_storer<Len1, Align1> storage_;");

    static_assert(storage_.Align <= std::alignment_of<AB_impl>::value, "Too large!=>  2nd template argument (Align1) of Aligned_storer<Len1, Align1> storage_;");
    static_assert(storage_.Align >= std::alignment_of<AB_impl>::value, "Too small!=>  2nd template argument (Align1) of Aligned_storer<Len1, Align1> storage_;");
    static_assert(storage_.Align == std::alignment_of<AB_impl>::value, "Please adjust 2nd template argument (Align1) of Aligned_storer<Len1, Align1> storage_;");

    static_assert(sizeof(storage_) == sizeof(         AB_impl), "Needs adjusting");

    new(&storage_) AB_impl(a, b);
}

template <typename T>
AB<T>::~AB()
{
    impl().~AB_impl();
}

template <typename T>
T AB<T>::get_a() const {
    return impl().get_a();
}

template <typename T>
T AB<T>::get_b() const {
    return impl().get_b();
}

template <typename T>
T AB<T>::get_sum() const {
    return impl().get_a() + impl().get_b();
}

template <typename T>
void AB<T>::inc_a_b() {
    impl().inc_a_b();
}


// https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl
// template instantiation
template class AB<int>;
template class AB<long double>;
