#include <chrono>
#include <iostream>
#include <thread>
#include <tuple>

#include "metainf.h"

using namespace std::literals;

struct timeout {
    using type = std::chrono::milliseconds;
};

class timeout_logger {
public:
    timeout_logger(std::chrono::milliseconds tm, const char* name):
        mDeadline(std::chrono::high_resolution_clock::now() + tm),
        mName(name)
    {}
    ~timeout_logger() {
        if (std::chrono::high_resolution_clock::now() > mDeadline)
            std::cerr << "Operation " << mName << " took longer than expected" << std::endl;
    }

private:
    std::chrono::high_resolution_clock::time_point mDeadline;
    const char* mName;
};

template<class C, typename R, typename... A>
R loggedRun(C&& c, member_ptr<std::decay_t<C>, R(A...)> m, A&&... a) {
    const auto* timeout_val = get_metainf<timeout>(m);
    if (!timeout_val)
        return (std::forward<C>(c).*m)(std::forward<A>(a)...);
    timeout_logger logger(*timeout_val, __PRETTY_FUNCTION__);
    return (std::forward<C>(c).*m)(std::forward<A>(a)...);
}

class Service {
public:
    void foo() {std::this_thread::sleep_for(1s); std::cout << __PRETTY_FUNCTION__ << std::endl;}
    void bar() {std::this_thread::sleep_for(1s); std::cout << __PRETTY_FUNCTION__ << std::endl;}
    void baz() {std::this_thread::sleep_for(1s); std::cout << __PRETTY_FUNCTION__ << std::endl;}
};
template<>const timeout::type member_metainf<timeout, decltype(&Service::foo), &Service::foo>::value = 50ms;
template<>const timeout::type member_metainf<timeout, decltype(&Service::bar), &Service::bar>::value = 2s;
template<>struct type_metainf<timeout, Service> {
    using members = std::tuple<
        member_metainf<timeout, decltype(&Service::foo), &Service::foo>,
        member_metainf<timeout, decltype(&Service::bar), &Service::bar>
    >;
};

int main() {
    Service srvc;
    loggedRun(srvc, &Service::foo);
    loggedRun(srvc, &Service::bar);
    loggedRun(srvc, &Service::baz);
    return EXIT_SUCCESS;
}
