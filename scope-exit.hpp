#pragma once
#include <utility>

template<typename F> struct scope_exit {
  scope_exit(F f): f(std::move(f)) {}
  ~scope_exit() { f(); }
  scope_exit(scope_exit&&) = delete;
private:
  F f;
};

template<typename F> struct cancellable_scope_exit {
  cancellable_scope_exit(F f): f(std::move(f)) {}
  ~cancellable_scope_exit() { run(); }

  cancellable_scope_exit(cancellable_scope_exit&& src):
    f(std::move(src.f)),
    active(std::exchange(src.active, false)) {}

  cancellable_scope_exit& operator=(cancellable_scope_exit& src) {
    f = std::move(src.f);
    active = std::exchange(src.f, false);
    return *this;
  }

  void cancel() {
    active = false;
  }

  void run() {
    if(active) {
      active = false;
      f();
    }
  }

private:
  F f;
  bool active = true;
};