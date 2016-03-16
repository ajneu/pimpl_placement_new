#include <new>
#include "ab.h"

// Implementation

class AB::AB_impl {
public:
    AB_impl(int a, int b) : a{a}, b{b}
    {  }

    int get_a() const { return a; }
    int get_b() const { return b; }
    void inc_a_b()    { a+=1; b+=1; }
   
private:
    int a;
    int b;
};


// Interface
AB::AB(int a, int b)
    : impl_{new(&storage_) AB_impl{a, b}, [](AB_impl *p) {p->~AB_impl(); } }
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

AB::~AB()
{
}

int AB::get_a() const {
    return impl_->get_a();
}

int AB::get_b() const {
    return impl_->get_b();
}

int AB::get_sum() const {
    return impl_->get_a() + impl_->get_b();
}

void AB::inc_a_b() {
    impl_->inc_a_b();
}
