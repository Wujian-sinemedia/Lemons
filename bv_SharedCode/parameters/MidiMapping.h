
namespace bav
{
    
    /*
        A simple listener class that allows you to map a specified parameter to a new MIDI controller number.
    */
    
    class MidiCC_Listener
    {
    public:
        MidiCC_Listener(Parameter* param, int controller): parameter(param), controllerNum(controller)
        {
            jassert (parameter != nullptr);
        }
        
        ~MidiCC_Listener() = default;
        
        void processMidiMessage (const juce::MidiMessage& msg)
        {
            if (msg.isController())
                processNewControllerMessage (msg.getControllerNumber(), msg.getControllerValue());
        }
        
        void processNewControllerMessage (int controllerNumber, int controllerValue)
        {
            constexpr auto inv127 = 1.0f / 127.0f;
            
            if (controllerNumber == controllerNum)
                parameter->changeValueNotifyHost (parameter->normalize (controllerValue * inv127));
        }
        
        void changeControllerNumber (int newControllerNumber) noexcept { controllerNum = newControllerNumber; }
        
        Parameter* getParameter() const noexcept { return parameter; }
        
        int getControllerNumber() const noexcept { return controllerNum; }
        
        
    private:
        Parameter* const parameter;
        int controllerNum;
    };
    
    
    
    
    /*
        A manager class that owns a set of MidiCCmapper objects and iterates through them to process all the CC changes in a midi buffer at a time
    */
    
    class MidiCC_MappingManager
    {
    public:
        MidiCC_MappingManager() { }
        
        ~MidiCC_MappingManager() { mappings.clear(); }
        
        /* adds a new MidiCC_Listener linked to the specified parameter and listening for the specified MIDI controller. */
        void addParameterMapping (Parameter* parameter, int controllerNumber)
        {
            mappings.add (new MidiCC_Listener (parameter, controllerNumber));
        }
        
        /* if a mapping exists for the current parameter, it will be changed to now listen for the new CC number. If no mapping exists for the parameter, returns false. */
        bool changeParameterMapping (Parameter* parameter, int newControllerNumber)
        {
            for (auto* mapping : mappings)
            {
                if (mapping->getParameter() == parameter)
                {
                    mapping->changeControllerNumber (newControllerNumber);
                    return true;
                }
            }
            return false;
        }
        
        /* removes all mappings to the passed parameter */
        void removeAllParameterMappingsFor (Parameter* parameter)
        {
            for (auto* mapping : mappings)
                if (mapping->getParameter() == parameter)
                    mappings.removeObject (mapping);
        }
        
        /* removes all mappings to the passed CC number */
        void removeAllParameterMappingsFor (int controllerNumber)
        {
            for (auto* mapping : mappings)
                if (mapping->getControllerNumber() == controllerNumber)
                    mappings.removeObject (mapping);
        }
        
        /* removes only mappings that map the passed parameter to the passed CC nnumber */
        void removeParameterMapping (Parameter* parameter, int controllerNumber)
        {
            for (auto* mapping : mappings)
                if (mapping->getParameter() == parameter && mapping->getControllerNumber() == controllerNumber)
                    mappings.removeObject (mapping);
        }
        
        
        /* processes all the MIDI CC events in the passed midi buffer */
        void processMidiBuffer (juce::MidiBuffer& midiMessages)
        {
            std::for_each (midiMessages.cbegin(),
                           midiMessages.cend(),
                           [&] (const juce::MidiMessageMetadata& meta) { processMidiEvent (meta.getMessage()); });
        }
        
        /* processes a single MIDI event at a time */
        void processMidiEvent (const juce::MidiMessage& msg)
        {
            if (msg.isController())
                processNewControllerMessage (msg.getControllerNumber(), msg.getControllerValue());
        }
        
        /* processes raw MIDI CC data. You can call this function manually to simulate the effect of recieving a MIDI CC message. */
        void processNewControllerMessage (int controllerNumber, int controllerValue)
        {
            for (auto* mapping : mappings)
                mapping->processNewControllerMessage (controllerNumber, controllerValue);
        }
        
        
    private:
        juce::OwnedArray< MidiCC_Listener > mappings;
    };
    
}  // namespace
