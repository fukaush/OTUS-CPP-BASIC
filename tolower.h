#pragma once

#include <iterator>
#include <algorithm>
#include <string>

std::string tolower(const std::string &str) {
  std::string lower_str;

  std::transform(std::cbegin(str), std::cend(str),
                 std::back_inserter(lower_str),
                 [](unsigned char ch) { return std::tolower(ch); });

  return lower_str;
};