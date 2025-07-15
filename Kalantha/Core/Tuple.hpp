/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Meta/Concepts.hpp>
#include <Kalantha/Meta/TypeTraits.hpp>
#include <Kalantha/Core/Utility.hpp>
#include <Kalantha/Core/IntegerSequence.hpp>
BEGIN_NAMESPACE_KTA_

/// NthTypeAccessor --
/// Access the nth type from a given pack Types as NthTypeAccessor::Type.
template<usize n, typename ...Types>
struct NthTypeAccessor;

template<usize n, typename T, typename ...Types>
struct NthTypeAccessor<n, T, Types...> {
  using Type = typename NthTypeAccessor<n - 1, Types...>::Type;
};

template<typename T, typename ...Types>
struct NthTypeAccessor<0, T, Types...> {
  using Type = T;
};

/// Helper type alias for NthTypeAccessor
template<usize n, typename ...Types>
using NthType = typename NthTypeAccessor<n, Types...>::Type;

/// TupleStorage -- recursive storage helper for the main Tuple class.
/// Tuple uses this to recursively define data fields with a
/// user-provided pack of types.
template<typename ...Tail>
struct TupleStorage;

template<typename T, typename ...Tail>
struct TupleStorage<T, Tail...> {
  constexpr TupleStorage() = default;
  ~TupleStorage() = default;

  constexpr TupleStorage(TupleStorage&&)      = default;
  constexpr TupleStorage(const TupleStorage&) = default;

  constexpr TupleStorage& operator=(TupleStorage&&)      = default;
  constexpr TupleStorage& operator=(const TupleStorage&) = default;

  constexpr TupleStorage(T&& head, Tail&&... tail)
  : head_(kta::move(head)), tail_(kta::move(tail)...) {}

  constexpr TupleStorage(const T& head, const Tail&... tail)
  : head_(head), tail_(tail...) {}

  template<typename ...OTs>
  constexpr TupleStorage(const TupleStorage<OTs...>& other)
  : head_(other.head_), tail_(other.tail_) {}

  template<typename ...OTs>
  constexpr TupleStorage(TupleStorage<OTs...>&& other)
  : head_(kta::move(other.head_)), tail_(kta::move(other.tail_)) {}

  T head_;
  TupleStorage<Tail...> tail_;
};

template<>
struct TupleStorage<> {
  constexpr TupleStorage() = default;
  ~TupleStorage() = default;
};

/// NthItemAccessor --
/// Access the nth item inside of a TupleStorage with a given
/// pack of types Types.
template<usize n, typename ...Types>
struct NthItemAccessor;

template<usize n, typename T, typename ...Types>
struct NthItemAccessor<n, T, Types...> {
  constexpr static auto
  access(const TupleStorage<T, Types...>& store) -> const NthType<n - 1, Types...>& {
    return NthItemAccessor<n - 1, Types...>::access(store.tail_);
  }

  constexpr static auto
  access(TupleStorage<T, Types...>& store) -> NthType<n - 1, Types...>& {
    return NthItemAccessor<n - 1, Types...>::access(store.tail_);
  }
};

template<typename T, typename ...Types>
struct NthItemAccessor<0, T, Types...> {
  constexpr static auto
  access(TupleStorage<T, Types...>& store) -> T& {
    return store.head_;
  }

  constexpr static auto
  access(const TupleStorage<T, Types...>& store) -> const T& {
    return store.head_;
  }
};

template<typename ...Types>
class Tuple /* Tuple Implementation -- */ {
public:
  template<typename ...OTs>
  friend class Tuple;

  template<usize n, typename ...OTs>
  struct ConstructibleWith_ {
    static constexpr bool value
      = IsConstructible<NthType<n, Types...>, NthType<n, OTs...>>
      && ConstructibleWith_<n - 1, OTs...>::value;
  };

  template<typename ...OTs>
  struct ConstructibleWith_<0, OTs...> {
    static constexpr bool value
      = IsConstructible<NthType<0, Types...>, NthType<0, OTs...>>;
  };

  template<usize i>
  constexpr typename NthTypeAccessor<i, Types...>::Type& get() {
    static_assert(i < sizeof...(Types), "Out of bounds tuple access!");
    return NthItemAccessor<i, Types...>::access(store_);
  }

  template<usize i>
  constexpr const typename NthTypeAccessor<i, Types...>::Type& get() const {
    static_assert(i < sizeof...(Types), "Out of bounds tuple access!");
    return NthItemAccessor<i, Types...>::access(store_);
  }

  template<typename ...OTs>
    requires ConstructibleWith_<sizeof...(OTs) - 1, OTs...>::value
  constexpr Tuple(const Tuple<OTs...>& other) : store_(other.store_) {}

  template<typename ...OTs>
    requires ConstructibleWith_<sizeof...(OTs) - 1, OTs...>::value
  constexpr Tuple(Tuple<OTs...>&& other) noexcept : store_(kta::move(other.store_)) {}

  template<typename ...OTs>
    requires (sizeof...(OTs) > 0 && ConstructibleWith_<sizeof...(OTs) - 1, OTs...>::value)
  constexpr Tuple(OTs&&... args) : store_(kta::forward<OTs>(args)...) {}

 ~Tuple() = default;
  constexpr Tuple() = default;
private:
  TupleStorage<Types...> store_;
};

/// Helper functions for creating and manipulating tuples.
template<typename ...Args>
constexpr auto make_tuple(Args&&... args) -> Tuple<RemoveReference<Args>...> {
  return Tuple<RemoveReference<Args>...>{ kta::forward<Args>(args)... };
}

template<usize n, typename ...Args>
constexpr auto tuple_accessor(const Tuple<Args...>& tup) -> const NthType<n, Args...>& {
  return tup. template get<n>();
}

template<usize n, typename ...Args>
constexpr auto tuple_accessor(Tuple<Args...>&& tup) -> NthType<n, Args...>&& {
  return kta::move(tup. template get<n>());
}

/// Implementation details for apply.
namespace detail_
{
  template<typename F, typename ...Types, usize ...indices_>
  constexpr auto apply_impl(F&& functor, Tuple<Types...>&& tup, IntegerSequence<usize, indices_...>)
    -> decltype(functor(tup.template get<indices_>()...))
  {
    return functor(tup.template get<indices_>()...);
  }

  template<typename F, typename ...Types, usize ...indices_>
  constexpr auto apply_impl(F&& functor, const Tuple<Types...>& tup, IntegerSequence<usize, indices_...>)
    -> decltype(functor(tup.template get<indices_>()...))
  {
    return functor(tup.template get<indices_>()...);
  }
} //namespace detail_

template<typename F, typename ...Types>
constexpr auto apply(F&& functor, Tuple<Types...>&& tup) -> decltype(auto) {
  return detail_::apply_impl(
    kta::forward<F>(functor),
    kta::move(tup),
    MakeIntegerSequence<usize, sizeof...(Types)>{});
}

template<typename F, typename ...Types>
constexpr auto apply(F&& functor, const Tuple<Types...>& tup) -> decltype(auto) {
  return detail_::apply_impl(
    kta::forward<F>(functor),
    kta::move(tup),
    MakeIntegerSequence<usize, sizeof...(Types)>{});
}

END_NAMESPACE_KTA_

/// For structured bindings.
namespace std {
  template<typename ...Types>
  struct tuple_size<::kta::Tuple<Types...>> {
    static constexpr usize value = sizeof...(Types);
  };

  template<usize i, typename ...Types>
  struct tuple_element<i, ::kta::Tuple<Types...>> {
    using type = ::kta::NthType<i, Types...>;
  };
}
