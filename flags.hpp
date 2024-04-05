#pragma once
#include <array>
#include <string_view>

using namespace std::literals::string_view_literals;
static constexpr std::array<std::pair<std::string_view, bool>, 8> data{
    {{"black"sv, true},
     {"blue"sv, false},
     {"cyan"sv, true},
     {"green"sv, false},
     {"magenta"sv, false},
     {"red"sv, true},
     {"white"sv, false},
     {"yellow"sv, true}}};