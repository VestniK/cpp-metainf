#pragma once

#include "utils.h"

template<typename Member>
struct member_trait;

template<typename C, typename T>
struct member_trait<member_ptr<C, T>> {
    using class_type = C;
    using type = T;
    using pointer = member_ptr<C, T>;
};

template<typename T, typename = require<>>
struct is_member: public std::false_type {};

template<typename T>
struct is_member<T, require<
    typename member_trait<T>::class_type,
    typename member_trait<T>::type,
    typename member_trait<T>::pointer
>>: public std::true_type {};

template<typename T>
using Member = typename std::enable_if<is_member<T>::value, T>::type;
