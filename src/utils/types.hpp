#pragma once

#include <aabb_tree.hpp>
#include <indexer.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template<class T>
using DynArray = std::vector<T>;

using Id = biss::index_t;

using Idx = std::size_t;

using FlagsStorage = std::size_t;

template<class T>
using HashSet = std::unordered_set<T>;

template<class K, class V>
using HashMap = std::unordered_map<K, V>;

template<class V>
using Index = biss::Indexer<V>;

template<class V>
using AABB2DTree = biss::AABBTree<V, 2, float>;

constexpr auto FLOAT_NAN = std::numeric_limits<float>::signaling_NaN();