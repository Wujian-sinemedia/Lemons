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

namespace lemons::alg
{

template <class ContainerType, class T>
[[nodiscard]] constexpr bool contains (const ContainerType& container, const T& value)
{
	return std::find (container.begin(), container.end(), value) != container.end();
}

template <class ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr bool contains_if (const ContainerType& container, UnaryPredicate&& p)
{
	return std::find_if (container.begin(), container.end(), std::move (p)) != container.end();
}

template <class ContainerType, class T, class UnaryPredicate>
[[nodiscard]] constexpr T& contains_or (const ContainerType& container, T& defaultValue, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::move (p));

	if (res == container.end())
		return defaultValue;

	return *res;
}

template <class ContainerType, class T, class UnaryPredicate>
[[nodiscard]] constexpr const T& contains_or (const ContainerType& container, const T& defaultValue, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::move (p));

	if (res == container.end())
		return defaultValue;

	return *res;
}

template <class T, class ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr T contains_or_default (const ContainerType& container, UnaryPredicate&& p)
{
	static_assert (std::is_default_constructible_v<T>,
				   "T must be default constructible!");

	const auto res = std::find_if (container.begin(), container.end(), std::move (p));

	if (res == container.end())
		return {};

	return *res;
}

template <class T, class ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr T* contains_or_null (const ContainerType& container, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::move (p));

	if (res == container.end())
		return nullptr;

	return *res;
}

template <class ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr int num_of (const ContainerType& container, UnaryPredicate&& p)
{
	return static_cast<int> (std::count_if (container.begin(), container.end(), std::move (p)));
}

template <class ContainerType>
constexpr void removeDuplicates (ContainerType& container)
{
	container.erase (std::unique (container.begin(), container.end()), container.end());
}

template <class ContainerType>
constexpr void reverse (ContainerType& container)
{
	std::reverse (container.begin(), container.end());
}

template <class ContainerType>
constexpr void sort (ContainerType& container, bool forward = true)
{
	std::sort (container.begin(), container.end());

	if (! forward)
		reverse (container);
}

template <class ContainerType, class Comparison>
constexpr void sort (ContainerType& container, Comparison&& predicate, bool forward = true)
{
	std::sort (container.begin(), container.end(), std::move (predicate));

	if (! forward)
		reverse (container);
}

template <class Container1, class Container2, class Callable>
constexpr void call_both (const Container1& container1, const Container2& container2, Callable&& callable)
{
	for (auto first = container1.begin(), second = container2.begin();
		 first != container1.end() && second != container2.end();
		 ++first, ++second)
	{
		callable (*first, *second);
	}
}

}  // namespace lemons::alg
