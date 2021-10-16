
/** @ingroup ModulationManager
 *  @{
 */
#pragma once


namespace lemons::plugin
{
/** A modulation manager for a plugin.
    This class holds a collection of LFOs that can be mapped to any number of Parameter objects. \n
    This class is also serializable; the saving and loading of parameter connections works by using the ParameterList::getParameterWithName() function.
 */
class ModulationManager : public SerializableData
{
public:
    using LfoNamingFunc = std::function< String (int) >;

    /** Creates a modulation manager. */
    ModulationManager (ParameterList& listToUse,
                       int            initNumLfos     = 1,
                       LfoNamingFunc  namingFuncToUse = nullptr);

    void addAllParametersTo (juce::AudioProcessor& processor);

    void addAllParametersAsInternal();


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


    LFO& addLFO();

private:
    void serialize (TreeReflector& ref) final;

    /*------------------------------*/

    ParameterList& paramList;

    juce::OwnedArray< LFO > lfos;

    dsp::BasicProcessorBase dummyProcessor;

    LfoNamingFunc namingFunc;
};

}  // namespace lemons::plugin

/** @}*/
