#pragma once

#include <type_traits>

#include "utils.h"

template<class C, typename T>
using member_ptr = T C::*;

// Getter
template<typename Getter>
struct getter_info;

template<typename C, typename T>
struct getter_info<member_ptr<C, T() const>> {
    using class_type = C;
    using property_type = T;
};

template<typename C, typename T>
struct getter_info<member_ptr<C, const T&() const>> {
    using class_type = C;
    using property_type = T;
};

template<typename T, typename = require<>>
struct is_getter: public std::false_type {};

template<typename T>
struct is_getter<T, require<typename getter_info<T>::class_type, typename getter_info<T>::property_type>>: public std::true_type {};

template<typename T>
using Getter = typename std::enable_if<is_getter<T>::value, T>::type;

// Setter
template<typename Setter>
struct setter_info;

template<typename C, typename T>
struct setter_info<member_ptr<C, void(T)>> {
    using class_type = C;
    using property_type = T;
};

template<typename C, typename T>
struct setter_info<member_ptr<C, void(const T&)>> {
    using class_type = C;
    using property_type = T;
};

template<typename C, typename T>
struct setter_info<member_ptr<C, void(T&&)>> {
    using class_type = C;
    using property_type = T;
};

template<typename T, typename = require<>>
struct is_setter: public std::false_type {};

template<typename T>
struct is_setter<T, require<typename setter_info<T>::class_type, typename setter_info<T>::property_type>>: public std::true_type {};

template<typename T>
using Setter = typename std::enable_if<is_setter<T>::value, T>::type;

// Members and properties info

template<class C, typename T, member_ptr<C, T> M>
struct member_info {
    static const char* const name;
    static const T& get(const C& c) {return (c.*M);}
    template<typename U>
    static void set(C& c, U&& val) {(c.*M) = std::forward<U>(val);}
};

template<typename TG, Getter<TG> Getter, typename TS, Setter<TS> Setter>
struct property_info {
    static_assert(
        std::is_same<typename getter_info<TG>::class_type, typename setter_info<TS>::class_type>::value,
        "getter and setter are members of different classes"
    );
    static_assert(
        std::is_same<typename getter_info<TG>::property_type, typename setter_info<TS>::property_type>::value,
        "getter and setter works with properties of different types"
    );

    static const char* const name;
    static auto get(const typename getter_info<TG>::class_type& c)
        -> decltype((c.*Getter)()) {
        return (c.*Getter)();
    }
    template<typename U>
    static void set(typename setter_info<TS>::class_type& c, U&& val) {(c.*Setter)(std::forward<U>(val));}
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
