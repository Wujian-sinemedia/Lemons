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

#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace lemons::dsp
{

namespace detail
{

struct copy_tag;
struct move_tag;
struct constructible_tag;
struct assignable_tag;

template <typename T, typename Tag1, typename Tag2>
struct is_rt_safe : std::false_type
{
};

template <typename T>
struct is_rt_safe<T, copy_tag, assignable_tag> : std::integral_constant<bool, std::is_trivially_copy_assignable<T>::value>
{
};

template <typename T>
struct is_rt_safe<T, copy_tag, constructible_tag> : std::integral_constant<bool, std::is_trivially_copy_constructible<T>::value>
{
};

template <typename T>
struct is_rt_safe<T, move_tag, assignable_tag> : std::integral_constant<bool, std::is_trivially_move_assignable<T>::value>
{
};

template <typename T>
struct is_rt_safe<T, move_tag, constructible_tag> : std::integral_constant<bool, std::is_trivially_move_constructible<T>::value>
{
};


template <typename T, typename Tag>
struct is_rt_safe<std::vector<T>, move_tag, Tag> : is_rt_safe<T, move_tag, Tag>
{
};

template <typename T, typename Tag>
struct is_rt_safe<std::set<T>, move_tag, Tag> : is_rt_safe<T, move_tag, Tag>
{
};

template <typename T, typename U, typename Tag>
struct is_rt_safe<std::map<T, U>, move_tag, Tag> : std::integral_constant<bool, is_rt_safe<T, move_tag, Tag>::value && is_rt_safe<U, move_tag, Tag>::value>
{
};

template <typename T, typename U, typename Tag>
struct is_rt_safe<std::unordered_map<T, U>, move_tag, Tag> : std::integral_constant<bool, is_rt_safe<T, move_tag, Tag>::value && is_rt_safe<U, move_tag, Tag>::value>
{
};

template <typename T, typename Tag>
struct is_rt_safe<std::unordered_set<T>, move_tag, Tag> : is_rt_safe<T, move_tag, Tag>
{
};

}  // namespace detail


template <typename T>
struct is_realtime_copy_assignable final : detail::is_rt_safe<T, detail::copy_tag, detail::assignable_tag>
{
};

template <typename T>
struct is_realtime_copy_constructable final : detail::is_rt_safe<T, detail::copy_tag, detail::constructible_tag>
{
};

template <typename T>
struct is_realtime_move_assignable final : detail::is_rt_safe<T, detail::move_tag, detail::assignable_tag>
{
};

template <typename T>
struct is_realtime_move_constructable final : detail::is_rt_safe<T, detail::move_tag, detail::constructible_tag>
{
};


template <typename T>
[[nodiscard]] constexpr bool atomicIsLockFree()
{
	return std::atomic<T>::is_always_lock_free;
}

}  // namespace lemons::dsp
