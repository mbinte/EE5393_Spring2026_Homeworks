// comb.h - inlined combinatorial functions (from vanilla)
#ifndef __COMB_H
#define __COMB_H
#include <assert.h>

inline unsigned fact(unsigned n, unsigned m)
{
   assert(n >= m);
   if (m == 0) 
      return 1;
   else
      return n*fact(n-1, m-1);
}

inline unsigned choose(unsigned n, unsigned m)
{
   assert(n >= m);
   if (n < 2*m) {
      return fact(n, n-m)/fact(n-m, n-m);
   } else {
      return fact(n, m)/fact(m, m);
   }
}

inline int raise(int n, unsigned m)
{
   int v = 1;
   for (unsigned i = 0; i < m; i++) v *= n;
   return v;
}

#endif
