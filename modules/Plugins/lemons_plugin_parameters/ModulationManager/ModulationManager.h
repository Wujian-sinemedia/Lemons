#pragma once

#include <lemons_oscillators/lemons_oscillators.h>

namespace lemons::plugin
{
class ModulationManager : public SerializableData
{
public:
    //ModulationManager (ParameterList& listToUse);

    void prepare (int blocksize, double samplerate);

    void processNextChunk (int numSamples);


    struct LFO : SerializableData
    {
        LFO() = default;
        LFO (ParameterList& listToUse);

        void prepareToPlay (int numSamples, double samplerate);

        void prepareNextBlock (int numSamples);

        float getAndDontAdvance() const;
        float getAndAdvance();
        void  advance (int numTicks = 1);

        void processNextSample (bool advance = true);

        dsp::osc::ChoosableOscillator< float >& getOscillator();

        bool hasConnection (Parameter& parameter) const;

        void createOrEditConnection (Parameter& parameter, int amount);

        void removeConnection (Parameter& parameter);

        void removeInvalidConnections();

        struct Connection : SerializableData
        {
            Connection() = default;
            Connection (ParameterList& listToUse, Parameter& parameter, int amount = 100);

            void apply (float newLFOsample) const;

            Parameter* param {nullptr};
            int        percentEffect {100};

        private:
            void serialize (TreeReflector& ref) final;

            ParameterList* paramList;
        };

    private:
        void serialize (TreeReflector& ref) final;

        ParameterList* paramList;

        juce::OwnedArray< Connection > connections;

        dsp::osc::ChoosableOscillator< float > osc;

        int currentTick {0};

        AudioBuffer< float > storage;
    };

    LFO* getLFO (int index);
    LFO* getLFOofType (dsp::osc::OscType type);
    LFO* getLFOatFrequency (float freq);
    LFO* getLFOwithConnection (Parameter& param);

private:
    void serialize (TreeReflector& ref) final;

    /*------------------------------*/

    //ParameterList& paramList;

    OwnedArray< LFO > lfos;
};

}  // namespace lemons::plugin
