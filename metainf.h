#pragma once

#include "member.h"
#include "utils.h"

template<typename MetaTag, typename TM, Member<TM> M>
struct member_metainf {
    static const typename MetaTag::type value;
};
