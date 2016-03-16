#include "ab.h"
#include <iostream>

template<typename T>
void print(AB<T> &ab, std::ostream &os = std::cout)
{
    os << ab.get_a() << " + " << ab.get_b() << " == " << ab.get_sum() << std::endl;
}

int main() {
    {
        AB<int> x(10, 20);
        print(x);
        x.inc_a_b();
        print(x);
    }
    std::cout << std::endl;
    
    {
        AB<long double> y(10.5, 20.5);
        print(y);
        y.inc_a_b();
        print(y);
    }
}
