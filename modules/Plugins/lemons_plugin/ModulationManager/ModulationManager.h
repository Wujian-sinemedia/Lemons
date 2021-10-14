#pragma once

#include <lemons_oscillators/lemons_oscillators.h>

namespace lemons::plugin
{
/** A modulation manager for a plugin.
    This class holds a collection of LFOs that can be mapped to any number of Parameter objects. \n
    This class is also serializable; the saving and loading of parameter connections works by using the ParameterList::getParameterWithName() function.
 */
class ModulationManager : public SerializableData
{
public:
    /** Creates a modulation manager. */
    ModulationManager (ParameterList& listToUse);

    void prepare (int blocksize, double samplerate);

    void processNextChunk (int numSamples);
    
    void addAllParametersTo (juce::AudioProcessor& processor);

    /*----------------------------------------------------------------------------------------------------------*/

    /** An LFO, to be used by a ModulationManager object.
        Internally, this class uses a dsp::osc::ChoosableOscillator to generate its values.
     */
    struct LFO : SerializableData
    {
        /** Internally, LFOs use ChoosableOscillator to generate their values. */
        using Osc = dsp::osc::ChoosableOscillator< float >;
        
        /** Creates a new LFO. */
        LFO (ParameterList& listToUse, String metaParameterName = "MetaParameter");

        /** Prepares the LFO for processing. */
        void prepareToPlay (int numSamples, double samplerate);

        /** Returns a reference to the LFO's internal ChoosableOscillator object. You can then call methods on the oscillator to, for example, set its frequency, change the type, etc. */
        [[nodiscard]] Osc& getOscillator() noexcept;
        
        [[nodiscard]] DefaultMetaParameter& getParameter() noexcept;

        void addParameterTo (juce::AudioProcessor& processor);

        // // //

        void prepareNextBlock (int numSamples);

        void processNextSample (bool advance = true);


    private:
        void serialize (TreeReflector& ref) final;

        Osc osc;
        
        MetaParam param;

        int currentTick {0};

        AudioBuffer< float > storage;
    };

    /*----------------------------------------------------------------------------------------------------------*/

    /** Adds a new LFO to the modulation manager.
        The modulation manager will take ownership of the LFO passed to this function.
     */
    void addLFO (LFO* newLFO);

    /** Creates and adds a new LFO to the modulation manager.
        The arguments are all passed to the constructor of ModulationManager::LFO.
     */
    template < typename... Args >
    void addLFO (Args&&... args)
    {
        addLFO (new LFO (std::forward< Args > (args)...));
    }

    /** Returns the LFO at the given index in the modulation manager's vector of LFOs.
        The returned pointer may be null if an LFO does not exist at the requested index. \n \n
        This is the least preferable way to retrieve LFO objects; prefer to use getLFOofType(), getLFOatFrequency(), or getLFOwithConnection().
     */
    LFO* getLFO (int index);

    /** Returns the first LFO found with the given oscillator type.
        The returned pointer may be null if no LFOs have the requested type.
     */
    LFO* getLFOofType (dsp::osc::OscType type);

    /** Returns the first LFO found with the given frequency.
        The returned pointer may be null if no LFOs have the requested frequency.
     */
    LFO* getLFOatFrequency (float freq);

    /** Returns the first LFO found that is connected to the specified parameter.
        The returned pointer may be null if no LFOs have the requested connection.
     */
    LFO* getLFOwithConnection (Parameter& param);

private:
    void serialize (TreeReflector& ref) final;

    /*------------------------------*/

    ParameterList& paramList;

    OwnedArray< LFO > lfos;
};

}  // namespace lemons::plugin
