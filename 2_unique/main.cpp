#include "ab.h"
#include <iostream>


void print(AB &ab, std::ostream &os = std::cout)
{
    os << ab.get_a() << " + " << ab.get_b() << " == " << ab.get_sum() << std::endl;
}

int main() {
    AB x(10, 20);
    print(x);
    x.inc_a_b();
    print(x);
}
