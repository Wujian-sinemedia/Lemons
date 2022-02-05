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
[[nodiscard]] constexpr bool contains_if (const ContainerType& container, UnaryPredicate p)
{
	return std::find_if (container.begin(), container.end(), p) != container.end();
}

template <class ContainerType, class T, class UnaryPredicate>
[[nodiscard]] constexpr T& contains_or (const ContainerType& container, T& defaultValue, UnaryPredicate p)
{
	const auto res = std::find_if (container.begin(), container.end(), p);

	if (res == container.end())
		return defaultValue;

	return *res;
}

template <class T, class ContainerType, class UnaryPredicate,
		  std::enable_if_t<std::is_default_constructible_v<T>>* = nullptr>
[[nodiscard]] constexpr T contains_or_default (const ContainerType& container, UnaryPredicate p)
{
	const auto res = std::find_if (container.begin(), container.end(), p);

	if (res == container.end())
		return {};

	return *res;
}

template <class T, class ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr T* contains_or_null (const ContainerType& container, UnaryPredicate p)
{
	const auto res = std::find_if (container.begin(), container.end(), p);

	if (res == container.end())
		return nullptr;

	return *res;
}

template <class ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr int num_of (const ContainerType& container, UnaryPredicate p)
{
	return static_cast<int> (std::count_if (container.begin(), container.end(), p));
}

}  // namespace lemons::alg
