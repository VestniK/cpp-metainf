#pragma once

#include "reflect.h"
#include "utils.h"

//     Direct writable concept (requres operator<<(std::ostream&, const T&))
template<typename T, typename = require<>>
struct is_direct_writable: public std::false_type {};

template<typename T>
struct is_direct_writable<T, require<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>>: public std::true_type {};

template<typename T>
using DirectWritable = typename std::enable_if<is_direct_writable<T>::value, T>::type;

//     Reflec-writable concept (requires Reflectable but not DirectWritable)
template<typename T>
using ReflectWritable = typename std::enable_if<is_reflectable<T>::value && !is_direct_writable<T>::value, T>::type;

//     Writer class
template<typename T>
class Writer final {
public:
    static void write(std::ostream& out, const T& t) {
        static_assert(
            is_reflectable<T>::value || is_direct_writable<T>::value,
            "Writer can't write value of non-reflectable type without operator<<(std::ostream&, const T&) overload."
        );
        Writer writer(out);
        writer.write<T>(t);
    }

private:
    Writer(std::ostream& out): mOut(out) {}

    template<typename U>
    void write(const DirectWritable<U>& val) {
        mOut << val;
    }

    template<typename U>
    void write(const ReflectWritable<U>& val) {
        mOut << '{' << std::endl;
        write<U>(val, std::make_index_sequence<std::tuple_size<Members<U>>::value>());
        mOut << '}';
    }

    void swallow(std::initializer_list<int>) {}

    template<typename U, size_t... I>
    void write(const Reflectable<U>& val, std::index_sequence<I...>) {
        swallow({ (writeKV(std::tuple_element<I, Members<U>>::type::name, std::tuple_element<I, Members<U>>::type::get(val)), 0)... });
    }

    template<typename U>
    void writeKV(const char* key, const U& val) {
        mOut << key << ": ";
        write<U>(val);
        mOut << std::endl;
    }

private:
    std::ostream& mOut;
};
