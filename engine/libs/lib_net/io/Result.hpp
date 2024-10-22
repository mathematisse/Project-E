
#pragma once

#include "lib_net/result/Result.hpp"
#include <system_error>

namespace lnet::io {

template<typename T>
using Result = result::Result<T, std::error_code>;

} // namespace lnet::io