#include <iostream>
#include "IntList.h"

template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0, int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

template<int a, int b>
struct Plus
{
	static int const value = a + b;
};

template<int a, int b>
struct Minus
{
	static int const value = a - b;
};

template<typename D>
struct Quantity;

template<int ...I>
struct Quantity<IntList<I...>>
{
	Quantity() : _data(0) {};
	explicit Quantity(const double& v) : _data(v) {};

	double value() const { return _data; };

	Quantity<IntList<I...>> operator+ (Quantity<IntList<I...>> q1)
	{
		return Quantity<IntList<I...>>(_data + q1._data);
	}

	Quantity<IntList<I...>> operator- (Quantity<IntList<I...>> q1)
	{
		return Quantity<IntList<I...>>(_data - q1._data);
	}

	template <int ...K>
	Quantity<typename Zip<IntList<I...>, IntList<K...>, Plus>::type> operator* (Quantity<IntList<K...>> q)
	{
		return Quantity<typename Zip<IntList<I...>, IntList<K...>, Plus>::type>(_data * q._data);
	}

	template <int ...K>
	Quantity<typename Zip<IntList<I...>, IntList<K...>, Minus>::type> operator/ (Quantity<IntList<K...>> q)
	{
		return Quantity<typename Zip<IntList<I...>, IntList<K...>, Minus>::type>(_data / q._data);
	}

	double _data;
	using  Who_I_am = IntList<I...>;
};

template<int ...I>
Quantity<IntList<I...>> operator* (double d, Quantity<IntList<I...>> q)
{
	return Quantity<IntList<I...>>(d * q._data);
}

template<int ...I>
Quantity<IntList<I...>> operator* (Quantity<IntList<I...>> q, double d)
{
	return Quantity<IntList<I...>>(d * q._data);
}

template<int ...I>
Quantity<IntList<I...>> operator/ (Quantity<IntList<I...>> q, double d)			
{
	return Quantity<IntList<I...>>(q._data / d);
}

template<int ...I>
Quantity<typename Zip<Dimension<>, IntList<I...>, Minus>::type> operator/ (double d, Quantity<IntList<I...>> q)			
{
	return Quantity<typename Zip<Dimension<>, IntList<I...>, Minus>::type>(d/ q._data);
}

using NumberQ = Quantity<Dimension<>>;			 // число без размерности
using LengthQ = Quantity<Dimension<1>>;          // метры
using MassQ = Quantity<Dimension<0, 1>>;		 // килограммы
using TimeQ = Quantity<Dimension<0, 0, 1>>;      // секунды
using VelocityQ = Quantity<Dimension<1, 0, -1>>; // метры в секунду
using AccelQ = Quantity<Dimension<1, 0, -2>>;    // ускорение, метры в секунду в квадрате
using ForceQ = Quantity<Dimension<1, 1, -2>>;    // сила в ньютонах

template<typename T>
void check()
{
	std::cout << __FUNCSIG__ << std::endl;
}

int main()
{
	LengthQ l1(8.0);

	auto l3 = l1 / 3.0;

	std::cout << (l3).value() << std::endl;
	check<decltype(l3)>();

	check<ForceQ>();
}

