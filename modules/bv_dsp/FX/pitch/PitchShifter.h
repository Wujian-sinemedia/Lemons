
#pragma once

namespace bav::dsp::FX
{
//template < typename SampleType >
//class PitchShifter
//{
//    using AudioBuffer = juce::AudioBuffer< SampleType >;
//
//public:
//    PitchShifter()
//    {
//        leftEngine.initialize (44100.0, 512);
//        rightEngine.initialize (44100.0, 512);
//    }
//
//    void prepare (double samplerate)
//    {
//        leftEngine.prepare (samplerate);
//        rightEngine.prepare (samplerate);
//
//        const auto period = leftEngine.getDesiredPeriod();
//        const auto freq   = math::freqFromPeriod (samplerate, period);
//        sampleRate        = samplerate;
//        setDesiredPitch (float (freq));
//    }
//
//    void reset()
//    {
//        leftEngine.reset();
//        rightEngine.reset();
//    }
//
//    void setDesiredPitch (float desiredFrequencyHz)
//    {
//        const auto period = math::periodInSamples (sampleRate, desiredFrequencyHz);
//        leftEngine.setDesiredPeriod (period);
//        rightEngine.setDesiredPeriod (period);
//    }
//
//    void process (AudioBuffer& audio)
//    {
//        leftEngine.process (audio);
//        rightEngine.process (audio);
//    }
//
//private:
//    struct Engine : FIFOWrappedEngine< SampleType >
//    {
//        using AudioBuffer = juce::AudioBuffer< SampleType >;
//
//        Engine() : shifter (analyzer) { }
//
//        void initialized (int, double) override
//        {
//            analyzer.initialize();
//        }
//
//        void prepareToPlay (double samplerate) override
//        {
//            analyzer.setSamplerate (samplerate);
//            FIFOWrappedEngine< SampleType >::changeLatency (analyzer.getLatency());
//            shifter.prepare();
//        }
//
//        void resetTriggered() override
//        {
//            analyzer.reset();
//            shifter.reset();
//        }
//
//        void release() override
//        {
//            analyzer.releaseResources();
//            shifter.releaseResources();
//        }
//
//        void setDesiredPeriod (int newPeriod) { desiredPeriod = newPeriod; }
//
//        int getDesiredPeriod() const { return desiredPeriod; }
//
//        void renderBlock (const AudioBuffer& input, AudioBuffer& output, juce::MidiBuffer&) override
//        {
//            const auto numSamples = input.getNumSamples();
//            analyzer.analyzeInput (input.getReadPointer (0), numSamples);
//            shifter.getSamples (output.getWritePointer (0), numSamples, desiredPeriod);
//        }
//
//        void latencyChanged (int newInternalBlocksize) override
//        {
//            analyzer.prepare (newInternalBlocksize);
//        }
//
//    private:
//        PsolaAnalyzer< SampleType > analyzer;
//        PsolaShifter< SampleType >  shifter;
//
//        int desiredPeriod = 551;
//    };
//
//
//    Engine leftEngine, rightEngine;
//    double sampleRate = 0;
//};
//
//
//template class PitchShifter< float >;
//template class PitchShifter< double >;
//
//
//template < typename SampleType >
//class ReorderablePitchShifter : public PitchShifter< SampleType >,
//                                public ReorderableEffect< SampleType >
//{
//public:
//    ReorderablePitchShifter() { }
//
//protected:
//    void fxChain_process (juce::AudioBuffer< SampleType >& audio) override
//    {
//        PitchShifter< SampleType >::process (audio);
//    }
//
//    void fxChain_prepare (double samplerate, int) override
//    {
//        PitchShifter< SampleType >::prepare (samplerate);
//    }
//
//private:
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReorderablePitchShifter)
//};
//
//template class ReorderablePitchShifter< float >;
//template class ReorderablePitchShifter< double >;

}  // namespace bav::dsp::FX
