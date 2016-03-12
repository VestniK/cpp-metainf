#pragma once

#include "utils.h"

template<typename Getter>
struct getter_trait;

template<typename C, typename T>
struct getter_trait<member_ptr<C, T() const>> {
    using class_type = C;
    using property_type = T;
};

template<typename C, typename T>
struct getter_trait<member_ptr<C, const T&() const>> {
    using class_type = C;
    using property_type = T;
};

template<typename T, typename = require<>>
struct is_getter: public std::false_type {};

template<typename T>
struct is_getter<T, require<
    typename getter_trait<T>::class_type,
    typename getter_trait<T>::property_type
>>: public std::true_type {};

template<typename T>
using Getter = typename std::enable_if<is_getter<T>::value, T>::type;
