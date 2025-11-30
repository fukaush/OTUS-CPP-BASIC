#pragma once

#include <type_traits>

template <typename T, typename Enable = void>
class Generator;

template <typename T>
class Generator<T, typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, unsigned int>::value>::type> {
  T start, end, current;

public:
  Generator(T s, T e): start{s}, end{e}, current{s-1} {}
  ~Generator() = default;

  T next() {
    current++;

    if (current > end) {
      current = start;
    }

    return current;
  }

  void reset() {
    current = start;
  }
};