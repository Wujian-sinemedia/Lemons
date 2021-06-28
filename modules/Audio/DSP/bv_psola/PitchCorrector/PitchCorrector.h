#pragma once

/*
 This class attempts to guess the correct pitch for an input signal, and performs PSOLA pitch shifting to provide pitch corrected output.
 Intended for monophonic audio only.
 */

namespace bav::dsp::psola
{

template<typename SampleType>
class PitchCorrectorBase
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;
    using Analyzer    = dsp::psola::Analyzer< SampleType >;
    
    PitchCorrectorBase (Analyzer& analyzerToUse, const midi::PitchPipeline* pitchConverterToUse = nullptr);
    
    void processNextFrame (AudioBuffer& output);
    
    void prepare (double samplerate, int blocksize);
    
private:
    float getTargetFrequency();
    
    Analyzer&  analyzer;
    
    const midi::PitchPipeline* pitch;
    
    dsp::psola::Shifter< SampleType > shifter {analyzer};
    
    double sampleRate {0.};
};


template<typename SampleType>
class PitchCorrector : public PitchCorrectorBase<SampleType>
{
public:
    PitchCorrector (const midi::PitchPipeline* pitchConverterToUse = nullptr);
    
private:
    dsp::psola::Analyzer< SampleType > analyzer;
};

}
