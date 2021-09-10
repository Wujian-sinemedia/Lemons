#pragma once

#include <bv_oscillators/bv_oscillators.h>

namespace bav::plugin
{
template < typename SampleType >
class ModulationManagerProcessor;


class ModulationManager : public SerializableData
{
public:
    virtual ~ModulationManager() = default;


private:
    friend class ModulationManagerProcessor< float >;
    friend class ModulationManagerProcessor< double >;

    struct LFO : dsp::osc::ChoosableOscillator< float >
    {
        void prepareToPlay (int numSamples, double samplerate);

        void processNextBlock (int numSamples);

        float getAndDontAdvance() const;
        float getAndAdvance();
        void  advance();

    private:
        int currentTick {0};

        AudioBuffer< float > storage;
    };

    /*------------------------------*/

    void serialize (TreeReflector& ref);

    /*------------------------------*/

    OwnedArray< LFO > lfos;
};

}  // namespace bav::plugin
