
namespace bav
{
  
    /* 
        A base class for an LFO modulation source. LFOs should output values in the range 0.0f to 1.0f -- this will be fed straight to a parameter's normalizable range.
    */
    class LFO_ModSource
    {
    public:
        LFO_ModSource() { }
      
        bool isMappedTo (Parameter* param) { return destinations.contains (param); }
      
        void addMapping (Parameter* param) { destinations.add (param); }
      
        void removeMapping (Parameter* param) { destinations.removeAllInstancesOf (param); }
      
        void processNextSample()
        {
            const auto value = getNextSampleValue();
          
            for (auto paramPtr : destinations)
                paramPtr->changeValueNotifyHost (value);
        }
      
    private:
        float getNextSampleValue() 
        {
            return 0.0f;
        }
      
        juce::Array< Parameter* > destinations;
    };
  
  
  
    /*
        Mapper class that manages a collection of LFO_ModSource objects
    */
  
    class ModSourceManager
    {
    public:
        ModSourceManager() { }
      
        void processNextSample()
        {
            for (auto* source : modSources)
                source->processNextSample();
        }
      
        LFO_ModSource* getModSourceForParam (Parameter* param)
        {
            for (auto* source : modSources)
                if (source->isMappedTo (param))
                    return source;
          
            return nullptr;
        }
      
    private:
        juce::OwnedArray< LFO_ModSource > modSources;
    };
  
  
}  // namespace
