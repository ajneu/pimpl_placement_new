#include <new>
#include "ab.h"

// Implementation

class AB::AB_impl {
public:
    AB_impl(int a, int b) : a{a}, b{b}
    {  }

    int get_a() const { return a; }
    int get_b() const { return b; }
    void inc_a_b()    { ++a; ++b; }
   
private:
    int a;
    int b;
};


// Interface

AB::AB(int a, int b)
{
    new(&storage_) AB_impl(a, b);

#ifndef ALIGNINFO_GENERATION_BUSY
    static_assert(storage_.Len   <= sizeof(           AB_impl),        "Too large!=>  1st template argument (Len1)   of Aligned_storer<Len1, Align1> storage_;");
    static_assert(storage_.Len   >= sizeof(           AB_impl),        "Too small!=>  1st template argument (Len1)   of Aligned_storer<Len1, Align1> storage_;");
    static_assert(storage_.Len   == sizeof(           AB_impl),        "Please adjust 1st template argument (Len1)   of Aligned_storer<Len1, Align1> storage_;");

    static_assert(storage_.Align <= std::alignment_of<AB_impl>::value, "Too large!=>  2nd template argument (Align1) of Aligned_storer<Len1, Align1> storage_;");
    static_assert(storage_.Align >= std::alignment_of<AB_impl>::value, "Too small!=>  2nd template argument (Align1) of Aligned_storer<Len1, Align1> storage_;");
    static_assert(storage_.Align == std::alignment_of<AB_impl>::value, "Please adjust 2nd template argument (Align1) of Aligned_storer<Len1, Align1> storage_;");

    static_assert(sizeof(storage_) == sizeof(         AB_impl), "Needs adjusting");
#endif
}

AB::~AB()
{
    impl().~AB_impl();
}

int AB::get_a() const {
    return impl().get_a();
}

int AB::get_b() const {
    return impl().get_b();
}

int AB::get_sum() const {
    return impl().get_a() + impl().get_b();
}

void AB::inc_a_b() {
    impl().inc_a_b();
}
