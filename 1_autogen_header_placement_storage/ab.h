#ifndef AB_H
#define AB_H

#include <ab_impl_aligninfo.h> /* The real version of this header, is generated via cmake (and CMakeLists.txt) in the build directory!
				  
				  Angle brackets (#include <...>) are needed! Reason:
				  Searches FIRST:
				  * in include directories defined with -I and system-directories (e.g. /usr/include)
				  Only if then NOT YET FOUND:
				  * it searches in the same directory as the current file

				  We need this behaviour, for it to pick up the generated header, whose location is given with -I./build

				  (In contrast, using quotes (#include "..."), searches first in the same directory as the current file)
			       */


#include "aligned_storer.h"

void aligninfo_generator(); // friend function (in a generated executable called               build/gen_ab_impl_aligninfo.h.exe) used to access AB::AB_impl
//                                                        That executable will generate header     build/ab_impl_aligninfo.h

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
