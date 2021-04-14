
namespace bav
{
     
    /*
        Attachment class that links a parameter to a specified OSC string.
    */
    class OSC_Attachment  :   public juce::OSCReceiver::ListenerWithOSCAddress< juce::OSCReceiver::RealtimeCallback >
    {
    public:
        OSC_Attachment(Parameter* param, juce::OSCAddress addr): parameter(param), address(addr)
        {
            jassert (parameter != nullptr);
        }
      
        void oscMessageReceived (const juce::OSCMessage& message) override
        {
            if (message.isEmpty())
                 return;
             
            const auto argument = message[0];
             
            auto* rap = parameter->orig();
            jassert (rap != nullptr);
             
            if (argument.isFloat32())
                 rap->setValueNotifyingHost (rap->convertTo0to1 (argument.getFloat32()));
            else if (argument.isInt32())
                 rap->setValueNotifyingHost (rap->convertTo0to1 (float (argument.getInt32())));
        }
      
        Parameter* getParameter() const noexcept { return parameter; }
      
        juce::OSCAddress getOSCaddress() const noexcept { return address; }
      
    private:
        Parameter* const parameter;
        juce::OSCAddress address;
    };
  
  
 
    /*
        Mapper class that owns & manages a collection of OSC_Attachment objects and serves as the main OSC reciever 
    */
    class OSCMappingManager  :    public juce::OSCReceiver
    {
    public:
      OSCMappingManager() { }
      
      void addNewMapping (Parameter* parameter, juce::OSCAddress address)
      {
          auto* newMapping = mappings.add (new OSC_Attachment (parameter, address));
          juce::OSCReceiver::addListener (newMapping, address);
      }
      
      void removeMapping (OSC_Attachment* mapping)
      {
          juce::OSCReceiver::removeListener (mapping);
          mappings.removeObject (mapping);
      }
      
      OSC_Attachment* getMappingForParameter (Parameter* param) const
      {
          for (auto* mapping : mappings)
              if (mapping->getParameter() == param)
                  return mapping;
          
          return nullptr;
      }
      
      OSC_Attachment* getMappingForAddress (juce::OSCAddress address)
      {
          for (auto* mapping : mappings)
              if (mapping->getOSCaddress() == address)
                  return mapping;
                  
          return nullptr;
      }
      
      void clearAllMappings()
      {
          do {
              removeMapping (mappings.getFirst());
          } while (! mappings.isEmpty());
      }
      
     
    private:
      juce::OwnedArray< OSC_Attachment > mappings;
    };
  
  
}  // namespace
