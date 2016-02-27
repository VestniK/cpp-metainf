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

    int get_age() const {return age;}
    int& get_age() {return age;}
    void set_age(int val) {age = val;}

    template<typename Str>
    void set_name(Str&& str) {name = std::forward<Str>(str);}
};
template<>
const char* member_info<User, std::string, &User::name>::name = "name";
template<>
const char* member_info<User, int, &User::age>::name = "age";
template<>
const char* member_info<User, int() const, &User::get_age>::name = "get_age";
template<>
const char* member_info<User, int&(), &User::get_age>::name = "get_age";
template<>
const char* member_info<User, void(int), &User::set_age>::name = "set_age";

// Can't have pointer to template member but can have pointer to instantion of template member
template<>
const char* member_info<User, void(const std::string&), &User::set_name>::name = "set_name";
template<>
const char* member_info<User, void(std::string&&), &User::set_name>::name = "set_name";

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
    std::cout << member_info<User, int() const, &User::get_age>::name << std::endl;
    std::cout << member_info<User, int&(), &User::get_age>::name << std::endl;
    std::cout << member_info<User, void(int), &User::set_age>::name << std::endl;
    std::cout << member_info<User, void(const std::string&), &User::set_name>::name << std::endl;
    std::cout << member_info<User, void(std::string&&), &User::set_name>::name << std::endl;
    std::cout << member_info<PointF, double, &PointF::x>::name << std::endl;
    return 0;
}
