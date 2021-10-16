
/** @ingroup PitchCorrector
 *  @{
 */
#pragma once

/*
 This class attempts to guess the correct pitch for an input signal, and performs PSOLA pitch shifting to provide pitch corrected output.
 Intended for monophonic audio only.
 */

namespace lemons::dsp::psola
{
/** This class attempts to guess the correct pitch for a given input signal, and performs PSOLA pitch shifting to provide a pitch corrected output.
    This base class references an external Analyzer object. The PitchCorrector class holds its own Analyzer.
    @see Analyzer, Shifter, PitchCorrector
 */
template < typename SampleType >
class PitchCorrectorBase
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;
    using Analyzer    = dsp::psola::Analyzer< SampleType >;

    /** Creates a pitch corrector. */
    PitchCorrectorBase (Analyzer& analyzerToUse, const midi::PitchPipeline* pitchConverterToUse = nullptr);

    /** Destructor. */
    virtual ~PitchCorrectorBase() = default;

    /** Processes the next frame of audio. */
    void processNextFrame (AudioBuffer& output);

    /** Prepares the pitch corrector. */
    void prepare (double samplerate);

    /** Returns the pitch that the output is being corrected to, as a MIDI pitch. */
    int getOutputMidiPitch() const;

    /** Returns the pitch that the output is being corrected to, as a frequency in Hz. */
    float getOutputFreq() const;

    /** Returns the number of cents sharp that the input signal is from the pitch it's being corrected to. */
    int getCentsSharp() const;

private:
    float updateAndReturnFreq();
    float getCurrentInputMidipitch() const;
    int   getMidipitchToCorrectTo (float inputMidipitch) const;

    Analyzer& analyzer;

    const midi::PitchPipeline* pitch;

    dsp::psola::Shifter< SampleType > shifter {analyzer};

    double sampleRate {0.};

    int correctedNote {-1};  // the midi pitch that the input will be corrected to
    int centsSharp {-1};
};


/** This class attempts to guess the correct pitch for a given input signal, and performs PSOLA pitch shifting to provide a pitch corrected output.
    @see Analyzer, Shifter, PitchCorrector
 */
template < typename SampleType >
class PitchCorrector : public PitchCorrectorBase< SampleType >
{
public:
    PitchCorrector (const midi::PitchPipeline* pitchConverterToUse = nullptr);

private:
    dsp::psola::Analyzer< SampleType > analyzer;
};

}  // namespace lemons::dsp::psola

/** @}*/
