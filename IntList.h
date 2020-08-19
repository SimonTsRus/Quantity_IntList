#pragma once
#include <tuple>

template <int ...N>
struct IntList;

template <int M, int ...N>
struct IntList<M, N...>
{
	const static int Head = M;
	using Tail = IntList<N...>;
};

template <>
struct IntList<>
{
};


template<typename IL>
struct Length
{
	static const int value = 1 + Length<typename IL::Tail>::value;
};

template<>
struct Length<IntList<>>
{
	static int const value = 0;
};


template<int M, typename IL>
struct IntCons;

template<int M, int ...N>
struct IntCons<M, IntList<N...> >
{
	using type = IntList<M, N...>;
};

template <int N, int K = N>
struct Generate
{
	using type = typename IntCons<K - N, typename Generate<N - 1, K>::type>::type;
};

template <int N>
struct Generate<0, N>
{
	using type = IntList<>;
};

template<typename F, typename T, int ...I>
auto invok(F f, T t, IntList<I...> fake) -> decltype(f(std::get<I>(t)...))
{
	return f(std::get<I>(t)...);
}

template <typename F, typename ...Args>
auto apply(F f, std::tuple<Args... > tup) ->decltype(invok(f, tup, typename Generate<sizeof...(Args)>::type()))
{
	return invok(f, tup, typename Generate<sizeof...(Args)>::type());
}

template <typename F>
auto apply(F f, std::tuple<> tup) ->decltype(f())
{
	return f();
}

template <typename TL1, typename TL2, template<int, int> class FunBin>
struct Zip
{
	using type = typename IntCons<FunBin< TL1::Head, TL2::Head >::value, typename Zip<typename TL1::Tail, typename TL2::Tail, FunBin>::type>::type;
};

template <template<int, int> class FunBin>
struct Zip<IntList<>, IntList<>, FunBin>
{
	using type = IntList<>;
};
