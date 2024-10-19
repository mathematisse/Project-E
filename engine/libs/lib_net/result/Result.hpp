#pragma once

#include <stdexcept>
#include <type_traits>
#include <variant>

namespace net::result {

struct Void : std::monostate {
    using std::monostate::monostate;
};

// T and E can't be the same type
template<typename T, typename E>
    requires std::is_default_constructible_v<T> && std::is_default_constructible_v<E> &&
    (!std::is_same_v<T, E>)
class Result {
public:
    using ResultType = std::variant<T, E>;

    explicit Result(const T &value):
        result(value)
    {
    }
    explicit Result(const E &error):
        result(error)
    {
    }

    static Result<T, E> Success(const T &value) { return Result<T, E>(value); }
    static Result<T, E> Error(const E &error) { return Result<T, E>(error); }

    [[nodiscard]] bool isError() const { return std::holds_alternative<E>(result); }
    [[nodiscard]] bool isValue() const { return std::holds_alternative<T>(result); }

    // bool overload to check if the result is an error
    explicit operator bool() const { return isValue(); }

    T value() const
    {
        if (isError()) {
            throw std::runtime_error("Attempted to get value from an error result");
        }
        return std::get<T>(result);
    }

    E error() const
    {
        if (!isError()) {
            throw std::runtime_error("Attempted to get error from a value result");
        }
        return std::get<E>(result);
    }

    const T &getValue() const
    {
        if (isError()) {
            throw std::runtime_error("Attempted to get value from an error result");
        }
        return std::get<T>(result);
    }

    const E &getError() const
    {
        if (!isError()) {
            throw std::runtime_error("Attempted to get error from a value result");
        }
        return std::get<E>(result);
    }

    explicit operator E() const { return error(); }
    explicit operator T() const { return value(); }

    // Simple constructor to easely create Error results without having to specify the type
    template<typename U>
    static Result<T, E> Error(U &&error) { return Result<T, E>(E(std::forward<U>(error))); }

    template<typename U>
    static Result<T, E> Success(U &&value) { return Result<T, E>(T(std::forward<U>(value))); }

    // Simple constructor to easely create Error results without having to specify the type
    template<typename U>
    static Result<T, E> Error(const U &error) { return Result<T, E>(E(error)); }

    template<typename U>
    static Result<T, E> Success(const U &value) { return Result<T, E>(T(value)); }

private:
    ResultType result;
};

} // namespace net::result