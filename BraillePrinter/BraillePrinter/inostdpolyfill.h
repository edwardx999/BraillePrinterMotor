#pragma once
#ifndef INO_STD_POLYFILL_H
#define INO_STD_POLYFILL_H
#include <stdint.h>
namespace ino {
	template<typename T,size_t N>
	struct array {
		T _data[N];
		constexpr T* data()
		{
			return _data;
		}
		constexpr T const* data() const
		{
			return _data;
		}
		constexpr size_t size() const
		{
			return N;
		}
	};
	template<typename T>
	struct make_signed {
		using type=T;
	};
	template<>
	struct make_signed<unsigned int> {
		using type=int;
	};
	template<>
	struct make_signed<unsigned short> {
		using type=short;
	};
	template<>
	struct make_signed<unsigned char> {
		using type=signed char;
	};
	template<>
	struct make_signed<char> {
		using type=signed char;
	};
	template<>
	struct make_signed<unsigned long> {
		using type=long;
	};
	template<>
	struct make_signed<unsigned long long> {
		using type=long long;
	};

	template<typename T>
	using make_signed_t=typename make_signed<T>::type;

	template<typename T>
	struct make_unsigned {
		using type=T;
	};
	template<>
	struct make_unsigned<int> {
		using type=unsigned int;
	};
	template<>
	struct make_unsigned<short> {
		using type=unsigned short;
	};
	template<>
	struct make_unsigned<char> {
		using type=unsigned char;
	};
	template<>
	struct make_unsigned<signed char> {
		using type=unsigned char;
	};
	template<>
	struct make_unsigned<long> {
		using type=unsigned long;
	};
	template<>
	struct make_unsigned<long long> {
		using type=unsigned long long;
	};

	template<typename T>
	using make_unsigned_t=typename make_unsigned<T>::type;

	template<typename T>
	T&& declval();

	template<typename T>
	struct remove_reference {
		using type=T;
	};
	template<typename T>
	struct remove_reference<T&> {
		using type=T;
	};
	template<typename T>
	struct remove_reference<T&&> {
		using type=T;
	};

	template<typename T>
	using remove_reference_t=typename remove_reference<T>::type;

	template<typename T>
	struct remove_const {
		using type=T;
	};
	template<typename T>
	struct remove_const<T const> {
		using type=T;
	};

	template<typename T>
	using remove_const_t=typename remove_const<T>::type;

	template<typename T>
	struct remove_volatile {
		using type=T;
	};
	template<typename T>
	struct remove_volatile<T volatile> {
		using type=T;
	};

	template<typename T>
	using remove_volatile_t=typename remove_volatile<T>::type;

	template<typename T>
	struct remove_cv {
		using type=remove_volatile_t<remove_const_t<T>>;
	};

	template<typename T>
	using remove_cv_t=typename remove_cv<T>::type;

	template<typename T>
	struct remove_cvref {
		using type=remove_cv_t<remove_reference_t<T>>;
	};

	template<typename T>
	using remove_cvref_t=typename remove_cvref<T>::type;

	template<typename... Types>
	struct basic_common_type;

	template<>
	struct basic_common_type<> {};
	template<typename T>
	struct basic_common_type<T> {
		using type=typename remove_cvref<T>::type;
	};
	template<typename T,typename U>
	struct basic_common_type<T,U> {
		using type=typename remove_cvref<decltype(true?declval<T>():declval<U>())>::type;
	};
	template<typename T,typename U,typename... Rest>
	struct basic_common_type<T,U,Rest...>:basic_common_type<typename basic_common_type<T,U>::type,Rest...> {};

	template<typename... T>
	using basic_common_type_t=typename basic_common_type<T...>::type;
}
#endif