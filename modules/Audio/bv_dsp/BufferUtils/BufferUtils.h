#pragma once

namespace bav::dsp::buffers
{

template<typename SampleType>
void copy (const juce::AudioBuffer<SampleType>& source, juce::AudioBuffer<SampleType>& dest);

}
