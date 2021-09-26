#pragma once

#include <lemons_oscillators/lemons_oscillators.h>

namespace bav::plugin
{
template < typename SampleType >
class ModulationManagerProcessor;


class ModulationManager : public SerializableData
{
public:
    void setBlocksize (int newBlocksize);

private:
    friend class ModulationManagerProcessor< float >;
    friend class ModulationManagerProcessor< double >;

    struct LFO : dsp::osc::ChoosableOscillator< float >
    {
        void prepareToPlay (int numSamples, double samplerate);

        void processNextBlock (int numSamples);

        float getAndDontAdvance() const;
        float getAndAdvance();
        void  advance (int numTicks = 1);

    private:
        int currentTick {0};

        AudioBuffer< float > storage;
    };

    /*------------------------------*/

    void serialize (TreeReflector& ref);

    /*------------------------------*/

    OwnedArray< LFO > lfos;

    int blocksize {50};
};

}  // namespace bav::plugin
