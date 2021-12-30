#if ! LEMONS_UNIT_TESTS
  #error
#endif


namespace lemons::tests
{

template <typename SampleType>
PeakFinderTests<SampleType>::PeakFinderTests()
: DspTest (getDspTestName<SampleType> ("PSOLA grain detector tests"))
{
}

template <typename SampleType>
void PeakFinderTests<SampleType>::runTest()
{
    constexpr auto samplerate = 44100.;
    
    for (const auto freq : { 85., 215., 440., 755., 2036., 3531. })
    {
        const auto period = math::periodInSamples (samplerate, freq);
        
        beginTest ("Period: " + String (period) + " samples");
        
        const auto blocksize = period * 4;
        
        peakFinder.prepare (blocksize);
        
        audioStorage.setSize (1, blocksize);
        
        runOscillatorTest (sine, samplerate, freq, period, "Sine");
        runOscillatorTest (saw, samplerate, freq, period, "Saw");
        runOscillatorTest (square, samplerate, freq, period, "Square");
        runOscillatorTest (triangle, samplerate, freq, period, "Triangle");
    }
    
    const auto numSamples = audioStorage.getNumSamples();
    
//    {
//        const auto subtest = beginSubtest ("Random noise");
//
//        fillAudioBufferWithRandomNoise (audioStorage, getRandom());
//
//        runBufferTest (nextRandomPeriod (samplerate, 60, numSamples));
//    }
    
    {
        const auto subtest = beginSubtest ("Silence");
        
        audioStorage.clear();
        
        runBufferTest (nextRandomPeriod (samplerate, 60, numSamples));
    }
}

template <typename SampleType>
void PeakFinderTests<SampleType>::runOscillatorTest (dsp::osc::Oscillator<SampleType>& osc,
                                                     double                            samplerate,
                                                     double                            freq,
                                                     int                               period,
                                                     const String&                     waveName)
{
    const auto oscSubtest = beginSubtest (waveName + " oscillator");
    
    osc.setFrequency (static_cast<SampleType> (freq),
                      static_cast<SampleType> (samplerate));
    
    osc.getSamples (audioStorage);
    
    runBufferTest (period);
}

template <typename SampleType>
void PeakFinderTests<SampleType>::runBufferTest (int period)
{
    const auto blocksize = audioStorage.getNumSamples();
    
    const auto& indices = peakFinder.findPeaks (audioStorage.getReadPointer (0), blocksize, period);
    
    {
        const auto subtest = beginSubtest ("Grains identified");
        expectGreaterOrEqual (indices.size(), (blocksize / period) - 1);
    }
    
    {
        const auto subtest = beginSubtest ("Indices are always increasing");
        
        for (int i = 0; i < indices.size() - 1; ++i)
            expectGreaterThan (indices.getUnchecked (i + 1),
                               indices.getUnchecked (i));
    }
    
    {
        const auto subtest = beginSubtest ("Max and min index are within range");
        
        expectGreaterOrEqual (indices.getUnchecked (0), 0);
        
        expectLessOrEqual (indices.getUnchecked (indices.size() - 1), blocksize);
    }
    
    const auto subtest = beginSubtest ("Grain spacing");
    
    /*
     Heuristics:
     - grains are approx 2 periods long
     - grains overlap by approx 50%
     - grains are centered on points of max energy in signal
     - individual grain onsets are approx 1 period apart
     */
    
    const auto halfPeriod = period / 2;
    
    for (int i = 0; i < indices.size() - 2; ++i)
    {
        const auto index1 = indices.getUnchecked (i);
        const auto index2 = indices.getUnchecked (i + 1);
        const auto index3 = indices.getUnchecked (i + 2);
        
        expectWithinAbsoluteError (index3 - index1, period * 2, halfPeriod);
        
        expectWithinAbsoluteError (index2 - index1, period, halfPeriod);
        expectWithinAbsoluteError (index3 - index2, period, halfPeriod);
    }
}

template <typename SampleType>
int PeakFinderTests<SampleType>::nextRandomPeriod (double samplerate, int minHz, int numSamples)
{
    const auto maxPeriod = std::min (math::periodInSamples (samplerate, minHz), numSamples / 2);
    const auto minPeriod = std::min (numSamples / 4, maxPeriod - 1);
    
    return getRandom().nextInt ({ minPeriod, maxPeriod + 1 });
}

template struct PeakFinderTests<float>;
template struct PeakFinderTests<double>;

}  // namespace lemons::tests
