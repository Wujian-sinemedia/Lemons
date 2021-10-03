#pragma once

#include "ParameterHolder.h"
#include <lemons_dsp/lemons_dsp.h>


namespace lemons::plugin
{
/** This class manages a list of a plugin's parameters.
    This class allows you to mark some parameters as "internal", not to be shown in the DAW's list of automatable parameters. Parameters marked "internal" (ie, not automatable), will be added to a dummy internal audio processor inside the ParameterList. The benefit of this is that these internal parameters will not show up in your DAW's list of parameters for your plugin under any circumstances, but they will still function as if they were added in the normal manner. \n
    \n
    This class also allows you to easily serialize all of your parameters using my SerializableData API. \n
    \n
    I usually recommend creating a subclass of ParameterList for your product. For example:
    @code
    using namespace lemons;
 
    struct MyParameters : plugin::ParameterList
    {
        MyParameters() : ParameterList ("MyParameters")
        {
            add (reverbToggle, delayToggle);
 
            // maybe this is just for communication with the GUI?
            addInternal (feedbackLevel);
        }
 
        ToggleParam reverbToggle {"Reverb toggle", true};
        ToggleParam delayToggle {"Delay toggle", false};
 
        dbParam feedbackLevel {"Feedback", -6.f};
    };
    @endcode
    Then, in your processor, you can simply do:
    @code
    struct MyProcessor : juce::AudioProcessor
    {
        MyProcessor()
        {
            params.addParametersTo (*this);
        }
 
        MyParameters params;
    };
    @endcode
    You should usually only need one ParameterList for your plugin.
    @see Parameter, ParameterHolder
 */
class ParameterList : public SerializableData
{
public:
    /** Creates a parameter list.
        @param name The identifier used for the list of parameters as a whole in serialization.
        @param um Optional pointer to an UndoManager to use with all of your parameters. This can also be set after construction with setUndoManager().
     */
    ParameterList (juce::Identifier name = "Parameters", UndoManager* um = nullptr);

    /** Adds the list's managed parameters to a juce::AudioProcessor.
        You should first add all your parameters to the list using add() or addInternal(), then call this to add all the parameters to your processor. \n
        \n
        Internal parameters will be added to a dummy internal AudioProcessor inside the ParameterList, so that they will not show up in your AudioProcessor's parameter list under any circumstances.
     */
    void addParametersTo (juce::AudioProcessor& processor);

    /** Adds all the list's parameters to the ParameterList's dummy internal AudioProcessor.
        This is useful for situations where you may want to instantiate the state of your plugin without creating the rest of your processor -- perhaps, in a GUI-only app version, or something like that. Simply call this function in the constructor of your app's main component and your plugin's state will be ready to use normally, as if it's living inside your processor.
     */
    void addAllParametersAsInternal();

    /** Calls Parameter::refreshDefault() on all the list's parameters.
        This sets all the parameters' default values to be their current values. \n
        \n
        This is useful, for example, after loading a preset: load the new state, then call this function, then any calls to Parameter::resetToDefault() will set the parameter to its value from the last loaded preset.
     */
    void refreshAllDefaults();

    /** Calls Parameter::resetToDefault() on all the list's parameters.
        This sets all the parameters back to their saved default values.
     */
    void resetAllToDefault();

    /** Sets the UndoManager to be used by all the list's parameters.
     */
    void setUndoManager (UndoManager& um);

    /** Adds a single parameter to the list.
        The parameter must live inside a ParameterHolder object, and will be added as a normal (ie, NOT "internal") parameter.
     */
    void add (ParamHolderBase& param);

    /** Adds a list of parameters to the list.
        You can call this function with any number of ParameterHolder objects with any parameter types.
     */
    template < typename... Args >
    void add (ParamHolderBase& first, Args&&... rest)
    {
        add (first);
        add (std::forward< Args > (rest)...);
    }

    /** Adds a single parameter to the list as an "internal" parameter.
        The parameter must live inside a ParameterHolder object.
     */
    void addInternal (ParamHolderBase& param);

    /** Adds a list of parameters to the list as "internal" parameters.
        You can call this function with any number of ParameterHolder objects with any parameter types.
     */
    template < typename... Args >
    void addInternal (ParamHolderBase& first, Args&&... rest)
    {
        addInternal (first);
        addInternal (std::forward< Args > (rest)...);
    }


    /** Sets a parameter to use to report the pitch wheel's current value.
        Calling this is optional, and mainly provided for convenience. \n \n
        After calling this, the IntParameter specified will always have the same value as the MIDI pitchwheel data coming into your plugin, and any listeners of that parameter will recieve appropriate callbacks when the pitchwheel is moved.
     */
    void setPitchbendParameter (IntParam& param);


    /** Sets a parameter to use to report the last moved MIDI CC number.
        Calling this is optional, and mainly provided for convenience. \n \n
        After calling this, the IntParameter specified will always have the same value as the last changed MIDI controller number. Any listners of this parameter will recieve appropriate callbacks each time the MIDI CC number changes.
     */
    void setLastMovedMidiControllerNumberParameter (IntParam& param);


    /** Sets a parameter to use to report the last moved MIDI CC value.
        Calling this is optional, and mainly provided for convenience. \n \n
        After calling this, the IntParameter specified will always have the same value as the last changed MIDI CC value. Any listners of this parameter will recieve appropriate callbacks each time the MIDI CC value changes.
     */
    void setLastMovedMidiControllerValueParameter (IntParam& param);


    /** Processes the next block of MIDI for your plugin.
        You must call this every processBlock() in order for the pitchbend and MIDI CC reporting parameters to work properly. \n \n
        This function processes all of the messages in the buffer at once; to get sample-accurate processing with smaller blocks rendered in between each MIDI message, use the ParameterProcessorBase class.
        @see ParameterProcessorBase
     */
    void processMidi (const MidiBuffer& midiMessages);

    /** Processes a single incoming MIDI message.
        You must call this every processBlock() in order for the pitchbend and MIDI CC reporting parameters to work properly.
     */
    void processMidiMessage (const MidiMessage& message);


    /** Class that fires a lambda callback each time any of its ParameterList's parameters change.
        This is useful mainly for detecting when a plugin's state has changed at all; for example, to tell when a preset has become dirty, etc. \n \n
        For more fine-grained reactions to individual parameters, I'd suggest using ParamUpdater.
        @see ParamUpdater
     */
    struct Listener
    {
        /** Creates a listener.
            @param list The ParameterList to listen to.
            @param onParamChange Lambda function that will be called when any of the ParameterList's parameters change.
            @param onGestureGhange Lambda function that will be called when any of the ParameterList's parameters' gesture states change.
            @param includeInternalParams When true, this Listener also recieves callbacks for all the ParameterList's "internal" parameters.
         */
        Listener (ParameterList& list,
                  std::function< void (Parameter&) >
                                                           onParamChange,
                  std::function< void (Parameter&, bool) > onGestureGhange       = {},
                  bool                                     includeInternalParams = true);

        /** Destructor. */
        virtual ~Listener() = default;

    private:
        OwnedArray< ParamUpdater > updaters;
    };

private:
    void serialize (TreeReflector& ref) final;

    void addParameter (ParamHolderBase& param, bool isInternal);

    void processNewControllerMessage (int controllerNumber, int controllerValue);
    void processNewPitchwheelMessage (int pitchwheelValue);

    std::vector< ParamHolderBase* > params;

    dsp::BasicProcessorBase dummyProcessor;

    IntParameter* pitchwheelParameter {nullptr};
    IntParameter* lastMovedControllerNumberParameter {nullptr};
    IntParameter* lastMovedControllerValueParameter {nullptr};

    UndoManager* undo {nullptr};
};


}  // namespace lemons::plugin
