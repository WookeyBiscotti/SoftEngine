#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <indexer.hpp>


template<class T>
using DynArray = std::vector<T>;
using Id = std::size_t;
using Idx = std::size_t;
using FlagsStorage = std::size_t;
template<class T>
using HashSet = std::unordered_set<T>;
template<class K, class V>
using HashMap = std::unordered_map<K, V>;
template<class V>
using Index = biss::Indexer<V>;

constexpr auto FLOAT_NAN = std::numeric_limits<float>::signaling_NaN();