#pragma once

#include <vector>

template<class T>
using DynArray = std::vector<T>;
using Id = std::size_t;
using FlagsStorage = std::size_t;

constexpr auto FLOAT_NAN = std::numeric_limits<float>::signaling_NaN();