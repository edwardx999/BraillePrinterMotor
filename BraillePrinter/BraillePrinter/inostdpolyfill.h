#pragma once
#ifndef INO_STD_POLYFILL_H
#define INO_STD_POLYFILL_H
namespace ino {
	
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
	struct common_type;

	template<>
	struct common_type<> {};

	template<typename T>
	struct common_type<T> {
		using type=remove_cvref_t<T>;
	};

	template<typename T,typename U>
	struct common_type<T,U> {
		using type=remove_cvref_t<decltype(false?declval<T>():declval<U>())>;
	};

	template<typename T,typename U,typename... Rest>
	struct common_type<T,U,Rest...>:common_type<typename common_type<T,U>::type,Rest...> {};

	template<typename... T>
	using common_type_t=typename common_type<T...>::type;

	template<typename T,size_t N>
	struct array {
		T _data[N];
		using value_type=remove_cvref_t<T>;
		using iterator=T*;
		using const_iterator=T const*;
		using reference=T&;
		using const_reference=T const&;
		using size_type=size_t;
		using difference_type=make_signed_t<size_type>;
		T* data()
		{
			return _data;
		}
		T const* data() const
		{
			return _data;
		}
		size_t size() const
		{
			return N;
		}
		T* begin()
		{
			return _data;
		}
		T* end()
		{
			return _data+N;
		}
		T const* begin() const
		{
			return _data;
		}
		T const* end() const
		{
			return _data+N;
		}
		reference operator[](size_t n)
		{
			return _data[n];
		}
		const_reference operator[](size_t n) const
		{
			return _data[n];
		}
	};
	template<typename T>
	class span {
	private:
		T* _data;
		size_t _count;
	public:
		span(T* data=nullptr,size_t count=0):_data{data},_count{count}{}
		using value_type=remove_cvref_t<T>;
		using iterator=T*;
		using const_iterator=T const*;
		using reference=T&;
		using const_reference=T const&;
		using size_type=size_t;
		using difference_type=typename make_signed<size_type>::type;
		T* data()
		{
			return _data;
		}
		T const* data() const
		{
			return _data;
		}
		size_t size() const
		{
			return _count;
		}
		T* begin()
		{
			return _data;
		}
		T* end()
		{
			return _data+_count;
		}
		T const* begin() const
		{
			return _data;
		}
		T const* end() const
		{
			return _data+_count;
		}
		reference operator[](size_t n)
		{
			return _data[n];
		}
		const_reference operator[](size_t n) const
		{
			return _data[n];
		}
	};

	template<typename Array>
	struct array_size;

	template<typename T,size_t N>
	struct array_size<T[N]> {
		static constexpr auto value=N;
	};

	template<typename T,size_t N>
	struct array_size<array<T,N>> {
		static constexpr auto value=N;
	};
}
#endif