#pragma once
#include <array>
#include <cstddef> // For std::size_t
#include <cstdint> // For std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t
#include <cstring> // For std::memset
#include <type_traits> // For std::conditional, std::is_same, std::is_same_v

template<typename T, typename... Types>
constexpr bool are_types_unique_v = (!std::is_same_v<T, Types> && ...) && are_types_unique_v<Types...>;

template<typename T>
constexpr bool are_types_unique_v<T> = true;

// Calculate bit position based on type index within the variadic list
template<typename T, typename... Structures>
struct BitPosition;

template<typename T, typename First, typename... Rest>
struct BitPosition<T, First, Rest...> : BitPosition<T, Rest...> { };

// TODO: add custom type for bitfield over 64 bits
template<typename T, typename... Rest>
struct BitPosition<T, T, Rest...> : std::integral_constant<uint64_t, 1ULL << sizeof...(Rest)> { };

template<typename T>
static constexpr size_t sizeInBits = sizeof(T) * 8;

// Custom type to store bitfields over 64 bits
template<size_t N>
class CustomSizeType {
    // must be a power of 2
    static constexpr size_t num_of_elements = (N % 64 == 0) ? N / 64 : N / 64 + 1;
    std::array<uint64_t, num_of_elements> data;

    CustomSizeType() { std::memset(data.data(), 0, data.size() * sizeof(uint64_t)); }

    // conversion from uint64_t to CustomSizeType
    CustomSizeType(uint64_t value)
    {
        std::memset(data.data(), 0, data.size() * sizeof(uint64_t));
        data[0] = value;
    }

    // conversion from CustomSizeType to uint64_t
    operator uint64_t() const { return data[0]; }

    // Binary operators
    CustomSizeType operator|(const CustomSizeType &rhs) const
    {
        CustomSizeType result;
        for (size_t i = 0; i < num_of_elements; ++i) {
            result.data[i] = data[i] | rhs.data[i];
        }
        return result;
    }

    CustomSizeType operator&(const CustomSizeType &rhs) const
    {
        CustomSizeType result;
        for (size_t i = 0; i < num_of_elements; ++i) {
            result.data[i] = data[i] & rhs.data[i];
        }
        return result;
    }

    CustomSizeType operator^(const CustomSizeType &rhs) const
    {
        CustomSizeType result;
        for (size_t i = 0; i < num_of_elements; ++i) {
            result.data[i] = data[i] ^ rhs.data[i];
        }
        return result;
    }

    CustomSizeType operator~() const
    {
        CustomSizeType result;
        for (size_t i = 0; i < num_of_elements; ++i) {
            result.data[i] = ~data[i];
        }
        return result;
    }

    CustomSizeType &operator|=(const CustomSizeType &rhs)
    {
        for (size_t i = 0; i < num_of_elements; ++i) {
            data[i] |= rhs.data[i];
        }
        return *this;
    }

    CustomSizeType &operator&=(const CustomSizeType &rhs)
    {
        for (size_t i = 0; i < num_of_elements; ++i) {
            data[i] &= rhs.data[i];
        }
        return *this;
    }

    CustomSizeType &operator^=(const CustomSizeType &rhs)
    {
        for (size_t i = 0; i < num_of_elements; ++i) {
            data[i] ^= rhs.data[i];
        }
        return *this;
    }

    CustomSizeType &operator<<=(size_t shift)
    {
        if (shift >= N) {
            std::memset(data.data(), 0, data.size() * sizeof(uint64_t));
        } else {
            size_t num_of_elements_to_shift = shift / sizeInBits<uint64_t>;
            size_t bits_to_shift = shift % sizeInBits<uint64_t>;
            for (size_t i = num_of_elements - 1; i >= num_of_elements_to_shift; --i) {
                data[i] = data[i - num_of_elements_to_shift] << bits_to_shift;
                if (i > num_of_elements_to_shift) {
                    data[i] |=
                        data[i - num_of_elements_to_shift - 1] >> (sizeInBits<uint64_t> - bits_to_shift);
                }
            }
            std::memset(data.data(), 0, num_of_elements_to_shift * sizeof(uint64_t));
        }
        return *this;
    }
};

template<class CustomSizeType>
auto ctzCustomSizeType(CustomSizeType &value) -> int
{
    for (size_t i = 0; i < value.num_of_elements; ++i) {
        if (value.data[i] != 0) {
            return __builtin_ctzll(value.data[i]) + i * sizeInBits<uint64_t>;
        }
    }
    // undefined behavior when the input is 0
    return int {};
}

template<size_t N>
constexpr auto select_storage_type()
{
    // using max_size =
    if constexpr (N <= sizeInBits<uint8_t>) {
        return uint8_t {};
    } else if constexpr (N <= sizeInBits<uint16_t>) {
        return uint16_t {};
    } else if constexpr (N <= sizeInBits<uint32_t>) {
        return uint32_t {};
    } else if constexpr (N <= sizeInBits<uint64_t>) {
        return uint64_t {};
#if defined(__SIZEOF_INT128__)
    } else if constexpr (N <= sizeInBits<unsigned __int128>) {
        return __int128 {};
#endif
#if defined(__SIZEOF_INT256__)
    } else if constexpr (N <= sizeInBits<__uint256_t>) {
        return __uint256_t {};
#endif
    } else {
        // static_assert(N <= sizeInBits<uint64_t>, "Number of structures exceeds the maximum storage size");
        return CustomSizeType<N> {0};
    }
}

// Structure container holding the bitfield and binary operations
template<typename... Structures>
    requires are_types_unique_v<Structures...>
class ComponentStatus {
public:
    static constexpr size_t num_of_structures = sizeof...(Structures);
    using storage_type = decltype(select_storage_type<num_of_structures>());

private:
    storage_type bitfield;

public:
    ComponentStatus():
        bitfield(0)
    {
    }

    template<typename T>
    inline void activate()
    {
        bitfield |= BitPosition<T, Structures...>::value;
    }

    template<typename T>
    inline void deactivate()
    {
        bitfield &= ~BitPosition<T, Structures...>::value;
    }

    template<typename T>
    [[nodiscard]] inline bool isActive() const
    {
        return (bitfield & BitPosition<T, Structures...>::value) != 0;
    }

    template<typename T>
    [[nodiscard]] inline size_t position() const
    {
        return BitPosition<T, Structures...>::value;
    }

    template<typename T>
    [[nodiscard]] inline size_t index() const
    {
        // ctz stands for count trailing zeros
        // __builtin_ctz is undefined behavior when the input is 0 but in this case it will never be 0
        // ctz is more appropriate for small bitfields

        if constexpr (sizeInBits<storage_type> <= sizeInBits<uint32_t>) {
            return __builtin_ctz(bitfield & BitPosition<T, Structures...>::value);
        } else if constexpr (sizeInBits<storage_type> <= sizeInBits<uint64_t>) {
            return __builtin_ctzll(bitfield & BitPosition<T, Structures...>::value);
        } else {

            return 0;
        }
    }

    [[nodiscard]] inline size_t size() const { return num_of_structures; }
    [[nodiscard]] inline size_t capacity() const { return sizeInBits<storage_type>; }
};
