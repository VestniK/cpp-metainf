#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "utils.h"
#include "reflect.h"
#include "write.h"

struct GeoPos {
    double lon;
    double lat;
};
template<>const char* member_info<GeoPos, double, &GeoPos::lon>::name = "lon";
template<>const char* member_info<GeoPos, double, &GeoPos::lat>::name = "lat";
template<>
struct type_info<GeoPos>{
    using members = std::tuple<
        member_info<GeoPos, double, &GeoPos::lon>,
        member_info<GeoPos, double, &GeoPos::lat>
    >;
};

struct User {
    std::string name;
    GeoPos location;
};
template<>const char* member_info<User, std::string, &User::name>::name = "name";
template<>const char* member_info<User, GeoPos, &User::location>::name = "location";
template<>
struct type_info<User>{
    using members = std::tuple<
        member_info<User, std::string, &User::name>,
        member_info<User, GeoPos, &User::location>
    >;
};

int main() {
    User vasya = {"vasya", GeoPos{54.0, 85.0}};
    Writer<User>::write(std::cout, vasya);
    std::cout << std::endl;
    return 0;
}
