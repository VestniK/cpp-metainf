#pragma once

template<typename... T>
struct make_void {typedef void type;};

template<typename... T>
using require = typename make_void<T...>::type;

template<class C, typename T>
using member_ptr = T C::*;
