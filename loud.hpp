#pragma once
#include <cstdio>

struct loud_options {
  bool noexcept_ctor = true,
       noexcept_dtor = true,
       noexcept_copy = false,
       noexcept_move = true;
};

template<loud_options opt={}> class loud {
  void print(auto... args) const {
    printf("%p ", this);
    printf(args...);
    fputc('\n', stdout);
  }

public:
  loud() noexcept(opt.noexcept_ctor) { print("ctor"); }
  ~loud() noexcept(opt.noexcept_dtor) { print("dtor"); }

  loud(const loud& rhs) noexcept(opt.noexcept_copy) {
    print("copy <- %p", &rhs);
  }
  loud& operator=(const loud& rhs) noexcept(opt.noexcept_copy) {
    print("copy assign <- %p", &rhs);
    return *this;
  }

  loud(loud&& rhs) noexcept(opt.noexcept_move) {
    print("move <- %p", &rhs);
  }
  loud& operator=(loud&& rhs) noexcept(opt.noexcept_move) {
    print("move assign <- %p", &rhs);
    return *this;
  }
};