#pragma once

#include <tuple>
#include <type_traits>

#include "member.h"
#include "utils.h"

template<typename MetaTag, typename TM, Member<TM> M>
struct member_metainf {
    static const typename MetaTag::type value;
    static const Member<TM> member;
};

template<typename MetaTag, typename TM, Member<TM> M>
const Member<TM> member_metainf<MetaTag, TM, M>::member = M;

template<typename MetaTag, class C>
struct type_metainf;

template<typename MetaTag, class C>
using MembersMetainf = typename type_metainf<MetaTag, C>::members;

template<typename MetaTag, class C, typename = require<>>
struct has_metainf: public std::false_type {};

template<typename MetaTag, class C>
struct has_metainf<MetaTag, C, require<MembersMetainf<MetaTag, C>>>: public std::true_type {};

namespace detail {
template<typename MetaTag, typename M, typename MembersTuple, size_t Idx, typename = void>
struct metainf_getter {
static const typename MetaTag::type* get(Member<M> m) {
    if (m == std::tuple_element<Idx, MembersTuple>::type::member)
        return &std::tuple_element<Idx, MembersTuple>::type::value;
    return metainf_getter<MetaTag, M, MembersTuple, Idx + 1>::get(m);
}
};
// recursion termination
template<typename MetaTag, typename M, typename MembersTuple, size_t Idx>
struct metainf_getter<MetaTag, M, MembersTuple, Idx, typename std::enable_if<Idx >= std::tuple_size<MembersTuple>::value>::type> {
static const typename MetaTag::type* get(Member<M> m) {
    return nullptr;
}
};
}

template<typename MetaTag, typename M>
const typename MetaTag::type* get_metainf(M m) {
    static_assert(
        is_member<M>::value &&
        has_metainf<MetaTag, typename member_trait<M>::class_type>::value,
        "M must be a pointer to member of some class with metainf corresponding to MetaTag attached");
    return detail::metainf_getter<MetaTag, M, MembersMetainf<MetaTag, typename member_trait<M>::class_type>, 0>::get(m);
}
