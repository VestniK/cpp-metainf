#include <iostream>
#include <string>

template<typename C, typename T>
using member_ptr = T C::*;

namespace detail {

template<typename C, typename T>
struct member_info_base {
    using type = T;
    using class_type = C;
};

};

template<typename C, typename T, member_ptr<C, T> M>
struct member_info;

struct Point {
    double x;
    double y;
};
template<>
struct member_info<Point, double, &Point::x>: detail::member_info_base<Point, double> {
    constexpr static const char* name = "x";
};
template<>
struct member_info<Point, double, &Point::y>: detail::member_info_base<Point, double> {
    constexpr static const char* name = "y";
};

int main() {
    std::cout << member_info<Point, double, &Point::x>::name << std::endl;
    return 0;
}
