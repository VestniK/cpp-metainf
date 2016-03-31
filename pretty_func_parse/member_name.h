#include <experimental/string_view>

#include "member.h"

struct constexpr_char_traits: public std::char_traits<char> {
    static constexpr size_t length(const char* val) {
        size_t res = 0;
        for (; val[res] != '\0'; ++res)
            ;
        return res;
    }

    static constexpr int compare(const char* lhs, const char* rhs, std::size_t count) {
        for (size_t pos = 0; pos < count; ++pos) {
            if (lhs[pos] == rhs[pos])
                continue;
            return lhs[pos] - rhs[pos];
        }
        return 0;
    }
};

using string_view = std::experimental::basic_string_view<char, constexpr_char_traits>;

template<typename T, Member<T> SomeMemberPtr>
constexpr
string_view get_member_name() {
    string_view res = __PRETTY_FUNCTION__;
    constexpr string_view start_pattern = "SomeMemberPtr = &";
    res = res.substr(res.find(start_pattern) + start_pattern.size());
    res = res.substr(0, res.find_first_of(";]"));
    res = res.substr(res.rfind("::") + 2);
    return res;
}

// libstdc++ version of string_view::compare and all of the comparision and equality
// operators which use it are not marked constexpr as required in the final draft
// of the n4480. The code bellow only compiles with libc++ so it's commented out.
//
// static_assert(
//     get_member_name<decltype(&string_view::data), &string_view::data>() == "data",
//     "Unsupported __PRETTY_FUNCTION__ format. Please provide implementation for your compiler"
// );
