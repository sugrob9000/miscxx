#pragma once

// constant wrapper (value -> type mapping)
template<auto x> struct cw_t {
  using type = decltype(x);
  constexpr static type value = x;
  constexpr operator type() const { return x; }
};
template<auto x> constexpr inline cw_t<x> cw;

// any_cw is modelled only by instantiations of cw_t.
// there could be a different, broader concept that would be modelled by
// integral_constant etc., but we only want to define operators for cw_t
template<typename> constexpr bool is_any_cw_v = false;
template<auto x> constexpr bool is_any_cw_v<cw_t<x>> = true;
template<typename t> concept any_cw = is_any_cw_v<t>;

#define cw_unop(op)\
  consteval any_cw auto operator op(any_cw auto x) {\
    return cw<op x.value>;\
  }
#define cw_binop(op)\
  consteval any_cw auto operator op(any_cw auto x, any_cw auto y) {\
    /* parens to avoid parsing error with > and >> */\
    return cw<(x.value op y.value)>;\
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