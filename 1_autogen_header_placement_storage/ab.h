#ifndef AB_H
#define AB_H

#include <ab_impl_aligninfo.h> // the real version of this header, is generated via cmake (and CMakeLists.txt) in the build directory!


#include "aligned_storer.h"

void aligninfo_generator();

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
   
   Aligned_storer</*Len*/ aligninfo_Len_AB_impl, /*Align*/ aligninfo_Align_AB_impl > storage_;
   class AB_impl;
   
   AB_impl       &impl()       { return reinterpret_cast<AB_impl       &>(storage_); }
   AB_impl const &impl() const { return reinterpret_cast<AB_impl const &>(storage_); }

   friend void aligninfo_generator();
};

#endif
