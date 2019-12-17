#pragma once

namespace util {

template<class type1, class type2>
struct pair {
  type1 first;
  type2 second;

  bool operator == (const pair<type1, type2>& other) {
    return (first == other.first && second == other.second);
  }
  bool operator != (const pair<type1, type2>& other) {
    return !((*this) == other);
  }
};

}  // namespace util

enum boardType {F, B, S, D, P, C};

struct action {
  int button;
  int x;
  int y;

  bool operator == (const action& other) {
    return (button == other.button && x == other.x && y == other.y);
  }
  bool operator != (const action& other) {
    return !((*this) == other);
  }
};
