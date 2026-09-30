#pragma once

template<typename... f>
  struct overload: f... {
    using f::operator()...;
  };
template<typename... f>
  overload(f...) -> overload<f...>;