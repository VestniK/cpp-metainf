#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

// Generic metaprogramming tools
template<typename... T>
struct make_void {typedef void type;};

template<typename... T>
using require = typename make_void<T...>::type;

// Members reflection simulation tools
template<typename C, typename T>
using MemberPtr = T C::*;

template<typename C, typename T, MemberPtr<C, T> M>
struct MemberInfo {
    static const char* name;
    static const T& getValue(const C& c) {return (c.*M);}
    using type = T;
    using class_type = C;
};
template<typename T>
struct TypeInfo;

template<typename T>
using Members = typename TypeInfo<T>::members;

//     Reflectable concept (requires members to be enlisted)
template<typename T, typename = require<>>
struct IsReflectable: public std::false_type {};

template<typename T>
struct IsReflectable<T, require<Members<T>>>: public std::true_type {};

template<typename T>
using Reflectable = typename std::enable_if<IsReflectable<T>::value, T>::type;

// Writing tools

//     Direct writable concept (requres operator<<(std::ostream&, const T&))
template<typename T, typename = require<>>
struct IsDirectWritable: public std::false_type {};

template<typename T>
struct IsDirectWritable<T, require<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>>: public std::true_type {};

template<typename T>
using DirectWritable = typename std::enable_if<IsDirectWritable<T>::value, T>::type;

//     Reflec-writable concept (requires Reflectable but not DirectWritable)
template<typename T>
using ReflectWritable = typename std::enable_if<IsReflectable<T>::value && !IsDirectWritable<T>::value, T>::type;

//     Writer class
template<typename T>
class Writer final {
public:
    static void write(std::ostream& out, const T& t) {
        static_assert(
            IsReflectable<T>::value || IsDirectWritable<T>::value,
            "Writer can't write value of non-reflectable type without operator<<(std::ostream&, const T&) overload."
        );
        Writer writer(out);
        writer.write<T>(t);
    }

private:
    Writer(std::ostream& out): mOut(out) {}

    template<typename U>
    void write(const DirectWritable<U>& val) {
        mOut << val;
    }

    template<typename U>
    void write(const ReflectWritable<U>& val) {
        mOut << '{' << std::endl;
        write<U>(val, std::make_index_sequence<std::tuple_size<Members<U>>::value>());
        mOut << '}';
    }

    void swallow(std::initializer_list<int>) {}

    template<typename U, size_t... I>
    void write(const Reflectable<U>& val, std::index_sequence<I...>) {
        swallow({ (writeKV(std::tuple_element<I, Members<U>>::type::name, std::tuple_element<I, Members<U>>::type::getValue(val)), 0)... });
    }

    template<typename U>
    void writeKV(const char* key, const U& val) {
        mOut << key << ": ";
        write<U>(val);
        mOut << std::endl;
    }

private:
    std::ostream& mOut;
};

// User types

struct GeoPos {
    double lon;
    double lat;
};
template<>const char* MemberInfo<GeoPos, double, &GeoPos::lon>::name = "lon";
template<>const char* MemberInfo<GeoPos, double, &GeoPos::lat>::name = "lat";
template<>
struct TypeInfo<GeoPos>{
    using members = std::tuple<
        MemberInfo<GeoPos, double, &GeoPos::lon>,
        MemberInfo<GeoPos, double, &GeoPos::lat>
    >;
};

struct User {
    std::string name;
    GeoPos location;
};
template<>const char* MemberInfo<User, std::string, &User::name>::name = "name";
template<>const char* MemberInfo<User, GeoPos, &User::location>::name = "location";
template<>
struct TypeInfo<User>{
    using members = std::tuple<
        MemberInfo<User, std::string, &User::name>,
        MemberInfo<User, GeoPos, &User::location>
    >;
};

int main() {
    User vasya = {"vasya", GeoPos{54.0, 85.0}};
    Writer<User>::write(std::cout, vasya);
    std::cout << std::endl;
    return 0;
}
