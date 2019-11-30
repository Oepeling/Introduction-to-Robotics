#pragma once

namespace util {

template<class type1, class type2>
struct pair {
  type1 first;
  type2 second;

  bool operator != (const pair<type1, type2>& other) {
    return (first != other.first || second != other.second);
  }
};

}  // namespace util

struct action {
  int button;
  int x;
  int y;
};
