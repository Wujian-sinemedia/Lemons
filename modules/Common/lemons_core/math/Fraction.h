/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

namespace lemons::math
{

template <typename ValueType = int>
struct Fraction
{
	explicit constexpr Fraction (ValueType num, ValueType denom) noexcept
		: numerator (num), denominator (denom)
	{
		jassert (denominator != ValueType (0));
	}

	template <typename T>
	explicit constexpr Fraction (const Fraction<T>& other) noexcept
		: Fraction (static_cast<ValueType> (other.numerator), static_cast<ValueType> (other.denominator))
	{
	}

	template <typename T>
	constexpr Fraction& operator= (const Fraction<T>& other) noexcept
	{
		numerator	= static_cast<ValueType> (other.numerator);
		denominator = static_cast<ValueType> (other.denominator);
		return *this;
	}

	constexpr operator double() const noexcept
	{
		return static_cast<double> (numerator) / static_cast<double> (denominator);
	}

	constexpr operator float() const noexcept
	{
		return static_cast<float> (numerator) / static_cast<float> (denominator);
	}

	constexpr bool operator== (const Fraction& other) const noexcept
	{
		return (double) *this == (double) other;  // NOLINT
	}

	constexpr bool operator!= (const Fraction& other) const noexcept
	{
		return ! *this == other;
	}

	constexpr bool operator> (const Fraction& other) const noexcept
	{
		return (double) *this > (double) other;	 // NOLINT
	}

	constexpr bool operator< (const Fraction& other) const noexcept
	{
		return (double) *this < (double) other;	 // NOLINT
	}

	ValueType numerator, denominator;
};

}  // namespace lemons::math
