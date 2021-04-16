
namespace bav
{
     
    /*
        Attachment class that links a parameter to a specified OSC string.
    */
    class OSC_Attachment  :   public juce::OSCReceiver::ListenerWithOSCAddress< juce::OSCReceiver::RealtimeCallback >
    {
    public:
        OSC_Attachment(Parameter* param, juce::OSCAddress addr, bool enabledAtStart = true): parameter(param), address(addr)
        {
            jassert (parameter != nullptr);
             enabled.store (enabledAtStart);
        }
      
        void oscMessageReceived (const juce::OSCMessage& message) override
        {
            if (message.isEmpty() || ! enabled.load())
                 return;
             
            const auto argument = message[0];
             
            auto* rap = parameter->orig();
            jassert (rap != nullptr);
             
            if (argument.isFloat32())
                 rap->setValueNotifyingHost (rap->convertTo0to1 (argument.getFloat32()));
            else if (argument.isInt32())
                 rap->setValueNotifyingHost (rap->convertTo0to1 (float (argument.getInt32())));
            else if (argument.isString())
            {
                 const auto string = argument.getString();
                 
                 if (string.containsIgnoreCase ("reset") || string.containsIgnoreCase ("default"))
                      parameter->resetToDefault();
                 else if (string.containsIgnoreCase ("refreshdefault") || string.containsIgnoreCase ("updatedefault"))
                      parameter->refreshDefault();
            }
        }
      
        Parameter* getParameter() const noexcept { return parameter; }
      
        juce::OSCAddress getOSCaddress() const noexcept { return address; }
         
        void changeOSCaddress (const juce::OSCAddress& newAddress) { address = newAddress; }
         
        void setEnabled (bool isNowEnabled) { enabled.store (isNowEnabled); }
      
    private:
        Parameter* const parameter;
        juce::OSCAddress address;
        std::atomic<bool> enabled;
    };
  
  
 
    /*
        Mapper class that owns & manages a collection of OSC_Attachment objects and serves as the main OSC reciever 
    */
    class OSCMappingManager  :    public juce::OSCReceiver
    {
    public:
      OSCMappingManager(bool enabledAtStart = true) { enabled.store(enabledAtStart); }
      
      void addNewMapping (Parameter* parameter, juce::OSCAddress address)
      {
          auto* newMapping = mappings.add (new OSC_Attachment (parameter, address, enabled.load()));
          juce::OSCReceiver::addListener (newMapping, address);
      }
      
      void loadMappingSet (OSC_Attachment* oscMappings, int numMappings)
      {
          mappings.clear();
          mappings.ensureStorageAllocated (numMappings);

          for (int i = 0; i < numMappings; ++i)
          {
              auto* mapping = oscMappings + i;
              mappings.add (new OSC_Attachment (mapping->getParameter(), mapping->getOSCaddress(), enabled.load()));
          }
      }
      
      void changeMapping (OSC_Attachment* mapping, juce::OSCAddress newAddress)
      {
          juce::OSCReceiver::removeListener (mapping);
          mapping->changeOSCaddress (newAddress);
          juce::OSCReceiver::addListener (mapping, newAddress);
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
          while (! mappings.isEmpty() {
              removeMapping (mappings.getFirst());
          }
      }
      
      void setEnabled (bool shouldBeEnabled) 
      { 
          enabled.store (shouldBeEnabled); 
          
          for (auto* mapping : mappings)
              mapping->setEnabled (shouldBeEnabled);
      }
      
      bool areOscMessagesEnabled() const noexcept { return enabled.load(); }
      
     
    private:
      juce::OwnedArray< OSC_Attachment > mappings;
      std::atomic<bool> enabled;
    };
  
  
}  // namespace
