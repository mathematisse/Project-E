
#pragma once

#include "lib_net/result/Result.hpp"
#include <system_error>

namespace net::io {

template<typename T>
using Result = result::Result<T, std::error_code>;

} // namespace net::io