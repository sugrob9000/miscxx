// operator""_c implementation that produces constant_t (integers only).
// 0x100_c -> constant<256> etc

#pragma once
#include "cw.hpp"
#include <climits>
#include <string_view>

constexpr unsigned long long parse_integer_literal(std::string_view s) {
  // s has already gone through the compiler's parser
  // so we can be mostly optimistic here
  int base = s.starts_with("0x") ? (s.remove_prefix(2), 16)
           : s.starts_with("0b") ? (s.remove_prefix(2), 2)
           : s.starts_with('0')  ? 8
           : 10;
  unsigned long long r = 0;
  for(char c: s) {
    int digit = base;
    switch(c) {
      case '\'': continue;
      case '0'...'9': digit = c-'0'; break;
      case 'a'...'f': digit = c+10-'a'; break;
      case 'A'...'F': digit = c+10-'A'; break;
    }
    if(digit >= base)
      throw "bad digit in literal";
    if(__builtin_mul_overflow(r, base, &r)
        || __builtin_add_overflow(r, digit, &r))
      throw "literal too large";
  }
  return r;
}

template<char... chars> consteval auto operator""_c() {
  constexpr char arr[] = {chars...};
  constexpr auto n = parse_integer_literal({arr, sizeof...(chars)});

  if constexpr     (n <= INT_MAX)   return constant<(int) n>;
  else if constexpr(n <= LONG_MAX)  return constant<(long) n>;
  else if constexpr(n <= LLONG_MAX) return constant<(long long) n>;
  else                              return constant<n>;
}

#if 0
static_assert([]{
  #define test(x) \
    static_assert(x##_c == x);\
    static_assert(__is_same(decltype(x##_c)::type, decltype(x)));\
    static_assert(__is_same(const decltype(x##_c), decltype(constant<x>)));
  test(0);
  test(1);
  test(000);
  test(0777);
  test(10'000'000'000'000'000'000);
  test(0b1010101);
  test(0x1000000000000000);
  #undef test
  return 1;
}());
#endif