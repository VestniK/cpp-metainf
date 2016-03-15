#include <chrono>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "metainf.h"
#include "reflect.h"
#include "utils.h"
#include "write.h"

using namespace std::literals;

class GeoPos {
public:
    GeoPos(double lon, double lat): mLon(lon), mLat(lat) {}

    double getLon() const {return mLon;}
    void setLon(double val) {mLon = val;}

    double getLat() const {return mLat;}
    void setLat(double val) {mLat = val;}

private:
    double mLon;
    double mLat;
};
template<>const char* const property_info<decltype(&GeoPos::getLon), &GeoPos::getLon, decltype(&GeoPos::setLon), &GeoPos::setLon>::name = "lon";
template<>const char* const property_info<decltype(&GeoPos::getLat), &GeoPos::getLat, decltype(&GeoPos::setLat), &GeoPos::setLat>::name = "lat";
template<>
struct type_info<GeoPos>{
    using members = std::tuple<
        property_info<decltype(&GeoPos::getLon), &GeoPos::getLon, decltype(&GeoPos::setLon), &GeoPos::setLon>,
        property_info<decltype(&GeoPos::getLat), &GeoPos::getLat, decltype(&GeoPos::setLat), &GeoPos::setLat>
    >;
};

struct User {
    std::string name;
    GeoPos location;
};
template<>const char* const member_info<decltype(&User::name), &User::name>::name = "name";
template<>const char* const member_info<decltype(&User::location), &User::location>::name = "location";
template<>
struct type_info<User>{
    using members = std::tuple<
        member_info<decltype(&User::name), &User::name>,
        member_info<decltype(&User::location), &User::location>
    >;
};

struct timeout {
    using type = std::chrono::milliseconds;
};

class Service {
public:
    void foo() {std::cout << "foo";}
    void bar() {std::cout << "bar";}
};
template<>const timeout::type member_metainf<timeout, decltype(&Service::foo), &Service::foo>::value = 1min;
template<>const timeout::type member_metainf<timeout, decltype(&Service::bar), &Service::bar>::value = 30s;

int main() {
    User vasya = {"vasya", GeoPos{54.0, 85.0}};
    Writer<User>::write(std::cout, vasya);
    std::cout << std::endl;
    return 0;
}
