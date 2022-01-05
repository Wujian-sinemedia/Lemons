#if ! LEMONS_UNIT_TESTS
#error
#endif

namespace lemons::tests
{

template <typename SampleType>
struct OscillatorTests final : public DspTest
{
    explicit OscillatorTests()
    : DspTest (getDspTestName<SampleType> ("Oscillator tests"))
    {
    }

    
private:
    void runTest() final
    {
        beginTest ("Sine tests");
        runOscillatorTests (sine);
        
        beginTest ("Saw tests");
        runOscillatorTests (saw);
        
        beginTest ("Square tests");
        runOscillatorTests (square);
        
        //    beginTest ("Triangle tests");
        //    runOscillatorTests (triangle);
    }
    
    void runOscillatorTests (dsp::osc::Oscillator<SampleType>& osc)
    {
        for (const auto samplerate : getTestingSamplerates())
        {
            const auto samplerateSubtest = beginSubtest ("Samplerate: " + String (samplerate));
            
            for (const auto period : { 50, 119, 350, 531 })
            {
                const auto periodSubtest = beginSubtest ("Period: " + String (period));
                
                const auto blocksize = period * 4;
                
                storage.setSize (1, blocksize, true, true, true);
                storage.clear();
                
                const auto freq = math::freqFromPeriod (samplerate, period);
                
                logImportantMessage ("Frequency: " + String (freq));
                
                osc.setFrequency (static_cast<SampleType> (freq),
                                  static_cast<SampleType> (samplerate));
                
                osc.getSamples (storage);
                
                expect (! bufferIsSilent (storage));
                expect (allSamplesAreValid (storage));
                expect (noSamplesAreClipping (storage));
                
                
                const auto subtest = beginSubtest ("Test zero-crossings");
                
                const auto* samples = storage.getReadPointer (0);
                
                zeroCrossings.clearQuick();
                bool status = samples[0] > SampleType (0);
                
                for (int s = 0; s < blocksize; ++s)
                {
                    const auto current = samples[s] > SampleType (0);
                    
                    if (current != status)
                    {
                        zeroCrossings.add (s);
                        status = current;
                    }
                }
                
                expectGreaterThan (zeroCrossings.size(), 2);
                
                for (int i = 1; i < zeroCrossings.size(); ++i)
                {
                    expectWithinAbsoluteError (zeroCrossings.getUnchecked (i) - zeroCrossings.getUnchecked (i - 1),
                                               period / 2,
                                               3);
                }
            }
        }
    }
    
    dsp::osc::Sine<SampleType>     sine;
    dsp::osc::Saw<SampleType>      saw;
    dsp::osc::Square<SampleType>   square;
    dsp::osc::Triangle<SampleType> triangle;
    
    AudioBuffer<SampleType> storage;
    
    juce::Array<int> zeroCrossings;
};

template struct OscillatorTests<float>;
template struct OscillatorTests<double>;

LEMONS_CREATE_DSP_TEST (OscillatorTests)

}  // namespace lemons::tests
