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

namespace lemons::serializing
{

template <typename ElementType>
struct ArrayConverter final
{
	[[nodiscard]] static juce::Array<ElementType> fromVar (const juce::var& v)
	{
		if (const auto* arr = v.getArray())
		{
			juce::Array<ElementType> array;

			array.ensureStorageAllocated (arr->size());

			for (const auto& element : *arr)
				array.add (juce::VariantConverter<ElementType>::fromVar (element));

			return array;
		}

		jassertfalse;

		return {};
	}

	[[nodiscard]] static juce::var toVar (const juce::Array<ElementType>& a)
	{
		juce::Array<juce::var> array;

		array.ensureStorageAllocated (a.size());

		for (const auto& element : a)
			array.add (juce::VariantConverter<ElementType>::toVar (element));

		return { array };
	}
};


template <typename ElementType>
struct VectorConverter final
{
	[[nodiscard]] static std::vector<ElementType> fromVar (const juce::var& v)
	{
		if (const auto* arr = v.getArray())
		{
			std::vector<ElementType> array;

			array.reserve (arr->size());

			for (const auto& element : *arr)
				array.push_back (juce::VariantConverter<ElementType>::fromVar (element));  // cppcheck-suppress useStlAlgorithm

			return array;
		}

		jassertfalse;

		return {};
	}

	[[nodiscard]] static juce::var toVar (const std::vector<ElementType>& v)
	{
		juce::Array<juce::var> array;

		array.ensureStorageAllocated (v.size());

		for (const auto& element : v)
			array.add (juce::VariantConverter<ElementType>::toVar (element));

		return { array };
	}
};


template <typename ElementType, size_t NumElements>
struct StdArrayConverter final
{
	[[nodiscard]] static std::array<ElementType, NumElements> fromVar (const juce::var& v)
	{
		if (const auto* arr = v.getArray())
		{
			jassert (arr->size() == NumElements);

			std::array<ElementType, NumElements> array;

			for (auto i = 0; i < NumElements; ++i)
				array.at (i) = juce::VariantConverter<ElementType>::fromVar (arr->getUnchecked (i));

			return array;
		}

		jassertfalse;

		return {};
	}

	[[nodiscard]] static juce::var toVar (const std::array<ElementType, NumElements>& a)
	{
		juce::Array<juce::var> array;

		array.ensureStorageAllocated (NumElements);

		for (const auto& element : a)
			array.add (juce::VariantConverter<ElementType>::toVar (element));

		return { array };
	}
};


template <typename ElementType>
struct OwnedArrayConverter final
{
	static void fromVar (const juce::var& v, juce::OwnedArray<ElementType>& a)
	{
		if (const auto* arr = v.getArray())
		{
			for (int i = 0; i < arr->size(); ++i)
			{
				const auto obj = juce::VariantConverter<ElementType>::fromVar (arr->getUnchecked (i));

				if (i < a.size())
				{
					*a.getUnchecked (i) = obj;
				}
				else
				{
					a.add (new ElementType (obj));
				}
			}
		}
		else
		{
			jassertfalse;
		}
	}

	[[nodiscard]] static juce::var toVar (const juce::OwnedArray<ElementType>& a)
	{
		juce::Array<juce::var> array;

		array.ensureStorageAllocated (a.size());

		for (const auto* element : a)
			array.add (juce::VariantConverter<ElementType>::toVar (*element));

		return { array };
	}
};

}  // namespace lemons::serializing
