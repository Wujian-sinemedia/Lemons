
namespace bav
{
     
    /*
        Attachment class that links a parameter to a specified OSC string.
    */
    class OSC_Attachment  :   public juce::OSCReceiver::ListenerWithOSCAddress< RealtimeCallback >
    {
    public:
        OSC_Attachment(Parameter* param, juce::OSCAddress addr): parameter(param), address(addr)
        {
            jassert (parameter != nullptr);
        }
      
        void oscMessageReceived (const juce::OSCMessage& message) override
        {
            juce::ignoreUnused (message);
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
    class OSCMappingManager  :    juce::OSCReceiver
    {
    public:
      OSCMappingManager() { }
      
      void addNewMapping (Parameter* parameter, juce::OSCAddress address)
      {
          auto* newMapping = mappings.add (new OSC_Attachment (parameter));
          this->addListener (newMapping, address);
      }
      
      
    private:
      juce::OwnedArray< OSC_Attachment > mappings;
    };
  
  
}  // namespace
