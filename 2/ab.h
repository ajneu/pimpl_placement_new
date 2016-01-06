#include "aligned_storer.h"

class AB {
public:
   AB(int a, int b);

   int get_a()   const;
   int get_b()   const;
   int get_sum() const;
   void inc_a_b();

private:

   // Implementation (well hidden!)
   
   Aligned_storer</*Len*/ 8, /*Align*/ 4> storage_;
   class AB_impl;
   
   AB_impl       &impl()       { return reinterpret_cast<AB_impl       &>(storage_); }
   AB_impl const &impl() const { return reinterpret_cast<AB_impl const &>(storage_); }
};
