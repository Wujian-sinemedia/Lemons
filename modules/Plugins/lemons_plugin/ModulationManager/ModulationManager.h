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

    /*----------------------------------------------------------------------------------------------------------*/

    /** An LFO, to be used by a ModulationManager object.
        Internally, this class uses a dsp::osc::ChoosableOscillator to generate its values.
     */
    struct LFO : SerializableData
    {
        /** Internally, LFOs use ChoosableOscillator to generate their values. */
        using Osc = dsp::osc::ChoosableOscillator< float >;

        /** Creates an LFO. */
        LFO() = default;

        /** Prepares the LFO for processing. */
        void prepareToPlay (int numSamples, double samplerate);

        /** Returns a reference to the LFO's internal ChoosableOscillator object. You can then call methods on the oscillator to, for example, set its frequency, change the type, etc. */
        Osc& getOscillator();

        /** Returns true if any of the LFO's connections are connected to the given Parameter. */
        bool hasConnection (Parameter& parameter) const;

        /** Returns the "amount" set for the connection to the given parameter. If no connection to the given parameter exists, returns -1. */
        int getConnectionAmount (Parameter& parameter) const;

        /** Creates or edits a parameter connection.
            If a connection to the specified parameter already exists, the connection's amount will be set to the new amount supplied here. Otherwise, a new connection will be added to this parameter.
         */
        void createOrEditConnection (Parameter& parameter, int amount);

        /** Removes the connection to the specified parameter, if one exists. */
        void removeConnection (Parameter& parameter);


        // // //

        void prepareNextBlock (int numSamples);

        void processNextSample (bool advance = true);


    private:
        friend class ModulationManager;

        LFO (ParameterList& listToUse);

        void removeInvalidConnections();

        void setParamList (ParameterList& listToUse);

        void serialize (TreeReflector& ref) final;

        struct Connection : SerializableData
        {
            Connection() = default;

            void apply (float newLFOsample) const;

            Parameter* param {nullptr};
            int        percentEffect {100};

        private:
            friend struct LFO;

            Connection (ParameterList& listToUse, Parameter& parameter, int amount = 100);

            void serialize (TreeReflector& ref) final;

            ParameterList* paramList;
        };

        ParameterList* paramList;

        juce::OwnedArray< Connection > connections;

        Osc osc;

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

    void removeAllInvalidConnections();

    /*------------------------------*/

    ParameterList& paramList;

    OwnedArray< LFO > lfos;
};

}  // namespace lemons::plugin
