#pragma once


namespace lemons
{

/** Returns true if this module was cross-compiled for iOS or Android, otherwise false. */
static constexpr bool isOnMobilePlatform()
{
#if (JUCE_IOS || JUCE_ANDROID)
	return true;
#else
	return false;
#endif
}

using juce::File;
using juce::String;
using juce::ValueTree;

template <typename T>
using Array = juce::Array<T>;

template <typename T>
using OwnedArray = juce::OwnedArray<T>;


struct SystemInitializer
{
	SystemInitializer();

	virtual ~SystemInitializer() = default;
};

}  // namespace lemons
