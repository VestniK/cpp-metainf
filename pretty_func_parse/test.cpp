#include <iostream>

#include "member_name.h"

std::ostream& operator<< (std::ostream& out, const string_view& val) {
    out.write(val.data(), val.size());
    return out;
}

template<typename T>
struct TPoint {
    T x = 0;
    T y = 0;
};

using Point = TPoint<int32_t>;
using PointF = TPoint<double>;

int main() {
    std::cout << get_member_name<decltype(&PointF::y), &PointF::y>() << std::endl;
    std::cout << get_member_name<decltype(&string_view::data), &string_view::data>() << std::endl;
    return EXIT_SUCCESS;
}
