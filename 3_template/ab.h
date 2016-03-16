#include "aligned_storer.h"

#include <cstddef>

template<typename T> class AB;

///// length and alignment information
template <typename TT>
class LenAlign {
    template<typename T> friend class AB;
    
    static const std::size_t len;
    static const std::size_t align;
};

template<>
class LenAlign<int> {
    template<typename T> friend class AB;
    
    static constexpr std::size_t len = 8;
    static constexpr std::size_t align = 4;
};

template<>
class LenAlign<long double> {
    template<typename T> friend class AB;
    
    static constexpr std::size_t len = 32;
    static constexpr std::size_t align = 16;
};


////// the actual class
template<typename T>
class AB {
public:
    AB(T a, T b);
    ~AB();
    T get_a()   const;
    T get_b()   const;
    T get_sum() const;
    void inc_a_b();
private:
    
    ///// Implementation (well hidden!)
    class AB_impl;
    Aligned_storer</*Len*/ LenAlign<T>::len, /*Align*/ LenAlign<T>::align> storage_; // this is the storage for AB_impl via placement new
    
    AB_impl       &impl()       { return reinterpret_cast<AB_impl       &>(storage_); }
    AB_impl const &impl() const { return reinterpret_cast<AB_impl const &>(storage_); }
};
