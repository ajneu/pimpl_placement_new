#include <memory>
#include "aligned_storer.h"

class AB {
public:
    AB(int a, int b);
    ~AB();
    int get_a()   const;
    int get_b()   const;
    int get_sum() const;
    void inc_a_b();
    
private:
    
    // Implementation (well hidden!)
    
    Aligned_storer</*Len*/ 8, /*Align*/ 4> storage_;
    class AB_impl;

    std::unique_ptr<AB_impl, void(*)(AB_impl *)> impl_;
};
