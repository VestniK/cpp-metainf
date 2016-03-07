#pragma once

#include <type_traits>

#include "utils.h"

template<typename C, typename T>
using member_ptr = T C::*;

template<typename C, typename T, member_ptr<C, T> M>
struct member_info {
    static const char* name;
    static const T& get(const C& c) {return (c.*M);}
    template<typename U>
    static void set(C& c, U&& val) {(c.*M) = std::forward<U>(val);}
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
