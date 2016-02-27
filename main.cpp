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

struct User {
    std::string name;
    int age;
};
template<>
const char* member_info<User, std::string, &User::name>::name = "name";
template<>
const char* member_info<User, int, &User::age>::name = "age";

template<typename T>
struct Point {
    T x;
    T y;
};

using PointF = Point<double>;
template<>
const char* member_info<PointF, double, &PointF::x>::name = "x";
template<>
const char* member_info<PointF, double, &PointF::y>::name = "y";

int main() {
    std::cout << member_info<User, std::string, &User::name>::name << std::endl;
    std::cout << member_info<PointF, double, &PointF::x>::name << std::endl;
    return 0;
}
