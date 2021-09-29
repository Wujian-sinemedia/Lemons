#pragma once

namespace lemons::dsp::buffers
{
template < typename SampleType >
void copy (const AudioBuffer< SampleType >& source, AudioBuffer< SampleType >& dest);

template < typename Type1, typename Type2 >
void convert (const AudioBuffer< Type1 >& source, AudioBuffer< Type2 >& dest);

}  // namespace lemons::dsp::buffers
