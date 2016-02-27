#include <iostream>
#include <string>

template<typename C, typename T>
using member_ptr = T C::*;

template<typename C, typename T, member_ptr<C, T> M>
struct member_info {
    static const char* name;
    using type = T;
    using class_type = C;
};

struct Point {
    double x;
    double y;
};
template<>
const char* member_info<Point, double, &Point::x>::name = "x";
template<>
const char* member_info<Point, double, &Point::y>::name = "y";

int main() {
    std::cout << member_info<Point, double, &Point::x>::name << std::endl;
    return 0;
}
