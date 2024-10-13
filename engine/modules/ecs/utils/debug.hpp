
#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <tuple>

namespace dbg {

// Helper to print a tuple (used for multiple member variables)
template<std::size_t Index = 0, typename... Types>
    requires(Index == sizeof...(Types))
auto print_tuple(std::ostream &os, const std::tuple<Types...> &) -> std::ostream &
{
    return os;
}

template<std::size_t Index = 0, typename... Types>
    requires(Index < sizeof...(Types))
constexpr auto print_tuple(std::ostream &os, const std::tuple<Types...> &t) -> std::ostream &
{
    os << (Index != 0 ? ", " : "") << std::get<Index>(t);
    return print_tuple<Index + 1>(os, t);
}

// Template function to generate the operator<< overload
template<typename T, typename... Args>
auto print(
    std::ostream &os, const T & /*obj*/, const std::string &className,
    const std::tuple<Args...> &args
) -> std::ostream &
{
    os << className << "(";
    print_tuple(os, args) << ")";
    return os;
}

} // namespace dbg

// Macro to automatically generate the operator<< overload for a struct
#define DERIVE_DEBUG(ClassName, ...)                                        \
    inline auto get_members() const { return std::tie(__VA_ARGS__); }       \
    friend std::ostream &operator<<(std::ostream &os, const ClassName &obj) \
    {                                                                       \
        return dbg::print(os, obj, #ClassName, obj.get_members());          \
    }
