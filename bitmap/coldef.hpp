#pragma once

#include <cinttypes>
#include <cstddef>

#include <concepts>
#include <stdexcept>

namespace coldef
{
#pragma pack(push, 1)
	template<typename T>
	struct BColor1
	{
		T val;

		constexpr operator T&() { return val; }
		constexpr operator const T() const { return val; }
		constexpr T& operator [](size_t index)
		{
			switch (index)
			{
			case 0: return val;
			default: throw std::invalid_argument("index must be one of 0, 1, 2");
			}
		}
		constexpr const T operator [](size_t index) const
		{
			switch (index)
			{
			case 0: return val;
			default: throw std::invalid_argument("index must be one of 0, 1, 2");
			}
		}
		static constexpr size_t size()
		{
			return 1;
		}
	};

	template<typename T>
	struct BColorRgb
	{
		T R;
		T G;
		T B;

		constexpr T& operator [](size_t index)
		{
			switch (index)
			{
			case 0: return R;
			case 1: return G;
			case 2: return B;
			default: throw std::invalid_argument("index must be one of 0, 1, 2");
			}
		}
		constexpr const T operator [](size_t index) const
		{
			switch (index)
			{
			case 0: return R;
			case 1: return G;
			case 2: return B;
			default: throw std::invalid_argument("index must be one of 0, 1, 2");
			}
		}
		static constexpr size_t size()
		{
			return 3;
		}
	};

	template<typename T>
	struct BColorRgba : public BColorRgb<T>
	{
		T A;

		constexpr T& operator [](size_t index)
		{
			switch (index)
			{
			case 0: return BColorRgb<T>::R;
			case 1: return BColorRgb<T>::G;
			case 2: return BColorRgb<T>::B;
			case 3: return A;
			default: throw std::invalid_argument("index must be one of 0, 1, 2, 3");
			}
		}
		constexpr const T operator [](size_t index) const
		{
			switch (index)
			{
			case 0: return BColorRgb<T>::R;
			case 1: return BColorRgb<T>::G;
			case 2: return BColorRgb<T>::B;
			case 3: return A;
			default: throw std::invalid_argument("index must be one of 0, 1, 2, 3");
			}
		}
		static constexpr size_t size()
		{
			return 4;
		}
	};
#pragma pack(pop)

	using Color1 = BColor1<uint8_t>;
	using ColorRgb =  BColorRgb<uint8_t>;
	using ColorRgba = BColorRgba<uint8_t>;
	using IColor1 = BColor1<int>;
	using IColorRgb = BColorRgb<int>;
	using IColorRgba = BColorRgba<int>;

	extern template struct BColor1<uint8_t>;
	extern template struct BColor1<int>;
	extern template struct BColorRgb<uint8_t>;
	extern template struct BColorRgba<uint8_t>;
	extern template struct BColorRgb<int>;
	extern template struct BColorRgba<int>;
};

