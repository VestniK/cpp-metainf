#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "utils.h"
#include "reflect.h"
#include "write.h"

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
template<>const char* const member_info<User, std::string, &User::name>::name = "name";
template<>const char* const member_info<User, GeoPos, &User::location>::name = "location";
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
