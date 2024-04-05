#pragma once
#include "flags.hpp"
#include <algorithm>
#include <array>
#include <stdexcept>
#include <string_view>

// Arrays magic
template <typename T, std::size_t N, std::size_t... Idx>
constexpr std::array<T, N> toStdArray(T *arr, std::index_sequence<Idx...>) {
  return {arr[Idx]...};
}

template <typename T, std::size_t N>
constexpr std::array<T, N> toStdArray(T *arr) {
  return toStdArray<T, N>(arr, std::make_index_sequence<N>());
}

template <typename T, std::size_t LeftSize, std::size_t RightSize,
          std::size_t... LeftIdx, std::size_t... RightIdx>
constexpr std::array<T, LeftSize + RightSize>
join(const std::array<T, LeftSize> &lhs, const std::array<T, RightSize> &rhs,
     std::index_sequence<LeftIdx...>, std::index_sequence<RightIdx...>) {
  return {lhs[LeftIdx]..., rhs[RightIdx]...};
}

template <typename T, std::size_t LeftSize, std::size_t RightSize>
constexpr std::array<T, LeftSize + RightSize>
join(const std::array<T, LeftSize> &lhs, const std::array<T, RightSize> &rhs) {
  return join(lhs, rhs, std::make_index_sequence<LeftSize>(),
              std::make_index_sequence<RightSize>());
}

// I can probably get rid of these 2 helper functions?
template <size_t Idx> constexpr auto getName() {
  return toStdArray<const char, data[Idx].first.size()>(data[Idx].first.data());
}

template <size_t Idx> constexpr auto getValue() {
  constexpr auto sv = data[Idx].second ? " TRUE"sv : " FALSE"sv;
  return toStdArray<const char, sv.size()>(sv.data());
}

template <size_t...> struct TableGeneratorStruct;
template <size_t First, size_t... Rest>
struct TableGeneratorStruct<First, Rest...> {
  static constexpr auto Value = join(
      join(TableGeneratorStruct<First>::Value, std::array<const char, 1>{0xA}),
      TableGeneratorStruct<Rest...>::Value);
};
template <size_t Idx> struct TableGeneratorStruct<Idx> {
  static constexpr auto Value = join(getName<Idx>(), getValue<Idx>());
};
template <size_t... ns>
constexpr auto genCompileFlagTable(std::index_sequence<ns...>) {
  return join(TableGeneratorStruct<ns...>::Value,
              std::array<const char, 1>{0x0});
}

// Checking Values

// Should it tell ya on-compile that flag is missing?
template <typename Key, typename Value, std::size_t Size> struct Map {
  std::array<std::pair<Key, Value>, Size> data;
  [[nodiscard]] constexpr Value at(const Key &key) const {
    const auto itr =
        std::find_if(begin(data), end(data),
                     [&key](const auto &v) { return v.first == key; });
    if (itr != end(data)) {
      return itr->second;
    } else {
      throw std::range_error("Not Found");
    }
  }
};

// endgame - std::array table
constexpr auto table =
    genCompileFlagTable(std::make_index_sequence<data.size()>());
constexpr std::string_view CompileFlagsTableView(table.data(), table.size());
// endgame2 - checks
constexpr bool isCompileFlag(const std::string_view sv) {
  return Map<std::string_view, bool, data.size()>{{data}}.at(sv);
}