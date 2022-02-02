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
struct ArrayConverter
{
	static juce::Array<ElementType> fromVar (const juce::var& v)
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

	static juce::var toVar (const juce::Array<ElementType>& a)
	{
		juce::Array<juce::var> array;

		array.ensureStorageAllocated (a.size());

		for (const auto& element : a)
			array.add (juce::VariantConverter<ElementType>::toVar (element));

		return { array };
	}
};

}  // namespace lemons::serializing
