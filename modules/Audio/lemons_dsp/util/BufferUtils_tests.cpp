#if ! LEMONS_UNIT_TESTS
#error
#endif

namespace lemons::tests
{

template <typename FloatType>
struct AudioBufferUtilsTests final : public DspTest
{
public:
    explicit AudioBufferUtilsTests()
    : DspTest (getDspTestName<FloatType> ("Audio buffer util functions"))
    {
    }
    
private:
    void runTest() final
    {
        for (const auto numChannels : { 1, 2, 3, 5, 8, 9 })
        {
            beginTest ("Num channels: " + String (numChannels));
            
            for (const auto numSamples : getTestingBlockSizes())
            {
                const auto blocksizeSubtest = beginSubtest ("Blocksize: " + String (numSamples));
                
                resizeAllBuffers (numSamples, numChannels);
                
                using namespace dsp::buffers;
                
                bufferA.clear();
                bufferB.clear();
                
                expect (bufferIsSilent (bufferA));
                expect (bufferIsSilent (bufferB));
                expect (buffersAreEqual (bufferA, bufferB));
                expect (noSamplesAreClipping (bufferA));
                
                bufferA.setSample (0, 1, FloatType (1.001));
                
                expect (! bufferIsSilent (bufferA));
                expect (! buffersAreEqual (bufferA, bufferB));
                expect (! noSamplesAreClipping (bufferA));
                
                fillAudioBufferWithRandomNoise (bufferA, getRandom());
                
                copy (bufferA, bufferB);
                
                expect (buffersAreEqual (bufferA, bufferB));
                
                copy (bufferB, bufferA);
                
                expect (buffersAreEqual (bufferA, bufferB));
                expect (allSamplesAreEqual (bufferA, bufferB, 0, numSamples));
                
                bufferA.setSample (0, 12, bufferA.getSample (0, 12) + FloatType (0.01));
                
                expect (! allSamplesAreEqual (bufferA, bufferB, 0, numSamples));
                expect (! buffersAreEqual (bufferA, bufferB));
                
                copy (bufferB, bufferA);
                
                expect (buffersAreEqual (bufferA, bufferB));
                
                const auto halfNumSamples = numSamples / 2;
                
                const auto aliasA = getAliasBuffer (bufferA, 0, halfNumSamples);
                
                expect (aliasA.getNumSamples() == halfNumSamples);
                expect (aliasA.getReadPointer (0) == bufferA.getReadPointer (0));
                expect (allSamplesAreEqual (aliasA, bufferB, 0, halfNumSamples));
                
                const auto aliasB = getAliasBuffer (bufferB, 0, halfNumSamples);
                
                expect (buffersAreEqual (aliasA, aliasB));
                
                bufferB.clear();
                
                expect (! buffersAreEqual (aliasA, aliasB));
                expect (! allSamplesAreZero (aliasA, 0, halfNumSamples));
                expect (! bufferIsSilent (aliasA));
                
                bufferA.clear();
                
                expect (buffersAreEqual (aliasA, aliasB));
                expect (allSamplesAreZero (aliasA, 0, halfNumSamples));
                expect (bufferIsSilent (aliasA));
                expect (bufferIsSilent (aliasB));
                
                if constexpr (std::is_same_v<FloatType, float>)
                    runConversionTests<double> (numChannels, numSamples);
                else
                    runConversionTests<float> (numChannels, numSamples);
                
                MidiBuffer midi;
                fillMidiBufferWithRandomEvents (midi, 25, getRandom());
                const auto copiedMidi = makeCopyOfMidiBuffer (midi);
                // expect (midiBuffersAreEqual (midi, copiedMidi));
            }
        }
    }
    
    template <typename OtherFloatType>
    void runConversionTests (int numChannels, int numSamples)
    {
        const auto subtest = beginSubtest ("Sample type conversion");
        
        AudioBuffer<OtherFloatType> otherBuffer { numChannels, numSamples };
        
        fillAudioBufferWithRandomNoise (bufferA, getRandom());
        
        const auto checkConvertedBuffers = [&] (const AudioBuffer<FloatType>& bufferToCheck)
        {
            for (int chan = 0; chan < numChannels; ++chan)
            {
                const auto* samplesA = bufferToCheck.getReadPointer (chan);
                const auto* samplesB = otherBuffer.getReadPointer (chan);
                
                for (int s = 0; s < numSamples; ++s)
                    expectWithinAbsoluteError (static_cast<FloatType> (samplesB[s]),
                                               samplesA[s],
                                               static_cast<FloatType> (0.001));
            }
        };
        
        dsp::buffers::convert (bufferA, otherBuffer);
        checkConvertedBuffers (bufferA);
        
        dsp::buffers::convert (otherBuffer, bufferB);
        checkConvertedBuffers (bufferB);
        
        expect (buffersAreReasonablyEqual (bufferA, bufferB));
    }
    
    void resizeAllBuffers (int numSamples, int numChannels)
    {
        jassert (numSamples > 0 && numChannels > 0);
        
        bufferA.setSize (numChannels, numSamples, true, true, true);
        bufferB.setSize (numChannels, numSamples, true, true, true);
    }
    
    AudioBuffer<FloatType> bufferA, bufferB;
};

template struct AudioBufferUtilsTests<float>;
template struct AudioBufferUtilsTests<double>;

LEMONS_CREATE_DSP_TEST (AudioBufferUtilsTests)

}  // namespace lemons::tests
