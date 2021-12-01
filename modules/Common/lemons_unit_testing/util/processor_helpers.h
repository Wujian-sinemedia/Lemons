#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace lemons::tests
{

using juce::String;


[[nodiscard]] bool allParameterNamesAreUnique (juce::AudioProcessor& processor);

[[nodiscard]] juce::AudioProcessorParameter* getNamedParameter (juce::AudioProcessor& processor,
                                                                const String& name);

struct ProcessorParameterData
{
    String name;
    float value;
};

using ProcessorParameterState = juce::Array<ProcessorParameterData>;

[[nodiscard]] ProcessorParameterState getStateOfProcessorParameters (juce::AudioProcessor& processor);

[[nodiscard]] bool processorMatchesParameterState (juce::AudioProcessor& processor,
                                                   const ProcessorParameterState& state);

}
