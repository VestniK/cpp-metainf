#pragma once

#include <type_traits>

#include "utils.h"

template<class C, typename T>
using member_ptr = T C::*;

template<class C, typename T, member_ptr<C, T> M>
struct member_info {
    static const char* const name;
    static const T& get(const C& c) {return (c.*M);}
    template<typename U>
    static void set(C& c, U&& val) {(c.*M) = std::forward<U>(val);}
};

template<class C, typename TGetter, TGetter Getter, typename TSetter, TSetter Setter>
struct property_info {
    // TODO: static asserts on TGetter and TSetter to check that they are getter and setter
    // in a class C
    static const char* const name;
    static auto get(const C& c) -> decltype((c.*Getter)()) {return (c.*Getter)();}
    template<typename U>
    static void set(C& c, U&& val) {(c.*Setter)(std::forward<U>(val));}
};

template<typename T>
struct type_info;

template<typename T>
using Members = typename type_info<T>::members;

//     Reflectable concept (requires members to be enlisted)
template<typename T, typename = require<>>
struct is_reflectable: public std::false_type {};

template<typename T>
struct is_reflectable<T, require<Members<T>>>: public std::true_type {};

template<typename T>
using Reflectable = typename std::enable_if<is_reflectable<T>::value, T>::type;
