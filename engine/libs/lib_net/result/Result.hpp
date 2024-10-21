#pragma once

#include <stdexcept>
#include <type_traits>
#include <variant>
#include <initializer_list>

namespace net::result {

struct Void : std::monostate {
    using std::monostate::monostate;
};

// T and E can't be the same type
template<typename T, typename E>
    requires(!std::is_same_v<T, E>)
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

    // Constructor to support brace-enclosed initializer list
    Result(std::initializer_list<T> valueList):
        result(*valueList.begin())
    {
        static_assert(valueList.size() == 1, "Initializer list must contain exactly one element");
    }

    Result(std::initializer_list<E> errorList):
        result(*errorList.begin())
    {
        static_assert(errorList.size() == 1, "Initializer list must contain exactly one element");
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

    // Simple constructor to easily create Error results without having to specify the type
    template<typename U>
    static Result<T, E> Error(U &&error)
    {
        return Result<T, E>(E(std::forward<U>(error)));
    }

    template<typename U>
    static Result<T, E> Success(U &&value)
    {
        return Result<T, E>(T(std::forward<U>(value)));
    }

    // Simple constructor to easily create Error results without having to specify the type
    template<typename U>
    static Result<T, E> Error(const U &error)
    {
        return Result<T, E>(E(error));
    }

    template<typename U>
    static Result<T, E> Success(const U &value)
    {
        return Result<T, E>(T(value));
    }

    // Map function to transform the value if it is a success
    template<typename F>
    auto map(F &&func) const -> Result<std::invoke_result_t<F, T>, E>
    {
        using NewT = std::invoke_result_t<F, T>;
        if (isValue()) {
            return Result<NewT, E>::Success(func(std::get<T>(result)));
        } else {
            return Result<NewT, E>::Error(std::get<E>(result));
        }
    }

    // Map function to transform the value if it is a failure
    template<typename F>
    auto map_error(F &&func) const -> Result<T, std::invoke_result_t<F, E>>
    {
        using NewE = std::invoke_result_t<F, E>;
        if (isError()) {
            return Result<T, NewE>::Error(func(std::get<E>(result)));
        } else {
            return Result<T, NewE>::Success(std::get<T>(result));
        }
    }

private:
    ResultType result;
};

} // namespace net::result