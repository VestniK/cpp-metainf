#pragma once

#include "utils.h"

template<typename Setter>
struct setter_trait;

template<typename C, typename T>
struct setter_trait<member_ptr<C, void(T)>> {
    using class_type = C;
    using property_type = T;
};

template<typename C, typename T>
struct setter_trait<member_ptr<C, void(const T&)>> {
    using class_type = C;
    using property_type = T;
};

template<typename C, typename T>
struct setter_trait<member_ptr<C, void(T&&)>> {
    using class_type = C;
    using property_type = T;
};

template<typename T, typename = require<>>
struct is_setter: public std::false_type {};

template<typename T>
struct is_setter<T, require<
    typename setter_trait<T>::class_type,
    typename setter_trait<T>::property_type
>>: public std::true_type {};

template<typename T>
using Setter = typename std::enable_if<is_setter<T>::value, T>::type;
