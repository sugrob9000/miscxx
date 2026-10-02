#pragma once

// constant wrapper (value -> type mapping)
template<auto x> struct constant_t {
  using type = decltype(x);
  constexpr static type value = x;
  constexpr operator type() const { return x; }
};
template<auto x> constexpr inline constant_t<x> constant;

// any_constant is modelled only by instantiations of constant_t.
// there could be a different, broader concept that would be modelled by
// integral_constant etc., but we only want to define operators for constant_t
template<typename> constexpr bool is_any_constant_v = false;
template<auto x> constexpr bool is_any_constant_v<constant_t<x>> = true;
template<typename t> concept any_constant = is_any_constant_v<t>;

#define cw_unop(op)\
  consteval auto operator op(any_constant auto x) {\
    return constant<op x.value>;\
  }
#define cw_binop(op)\
  consteval auto operator op(any_constant auto x, any_constant auto y) {\
    /* parens to avoid parsing error with > and >> */\
    return constant<(x.value op y.value)>;\
  }

cw_unop(+) cw_unop(-) cw_unop(!) cw_unop(~) cw_unop(*) cw_unop(&)
cw_binop(+) cw_binop(-) cw_binop(*) cw_binop(/) cw_binop(%)
cw_binop(|) cw_binop(&) cw_binop(^) cw_binop(&&) cw_binop(||)
cw_binop(->*) cw_binop(<<) cw_binop(>>)
cw_binop(<=>) cw_binop(==) cw_binop(!=)
cw_binop(<) cw_binop(<=) cw_binop(>) cw_binop(>=)
// ignore =, +=, ++ etc., comma...
#undef cw_unop
#undef cw_binop

// type -> value mapping
// (for passing types in normal function arguments instead of template arguments etc.)
template<typename t> struct type_t { using type = t; };
template<typename t> constexpr inline type_t<t> type_v;