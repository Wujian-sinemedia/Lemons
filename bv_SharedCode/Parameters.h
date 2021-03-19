

namespace bav
{
    
    /*
     this function attempts to return the default location your plugin's preset files should be saved to and loaded from.
     if the directory cannot be found for your plugin, calling this function will attempt to create it.
     */
    inline juce::File getPresetsFolder (std::string companyName, std::string pluginName)
    {
        juce::File rootFolder;
        
#if (BV_OSX || BV_POSIX || BV_IOS)
        rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userApplicationDataDirectory)
                        .getChildFile ("Audio").getChildFile ("Presets");
#elif (BV_LINUX || BV_BSD || BV_ANDROID)
        rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userApplicationDataDirectory);
#elif BV_WINDOWS
        rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDocumentsDirectory);
#else
  #error Unknown operating system!
#endif
        rootFolder = rootFolder.getChildFile (companyName).getChildFile (pluginName);
        
        if (! rootFolder.isDirectory())
            rootFolder.createDirectory(); // creates the presets folder if it doesn't already exist
        
        return rootFolder;
    }
    
    
    /*
     This function returns a given RangedAudioParameter's current normalized value in the range 0.0f to 1.0f.
     This function is necessary because the provided juce subclasses AudioParameterFloat|Bool|Int implement getValue() as private.
     */
    inline float getNormalizedParameterValue (juce::RangedAudioParameter& param)
    {
        return param.getValue();
    }
    
    
    /*
     Wrapper class around juce::AudioParameterFloat that allows you to change its default value at runtime, and exposes the getValue() function as public
     */
    class FloatParameter  :     public juce::AudioParameterFloat
    {
        using AudioParameterFloat = juce::AudioParameterFloat;
        
    public:
        // use the constructor just like you would the constructor for juce::AudioParameterFloat. All the args are simply forwarded.
        FloatParameter(juce::String parameterID, juce::String parameterName, juce::NormalisableRange<float> range, float defaultVal): AudioParameterFloat(parameterID, parameterName, range, defaultVal)
        {
            currentDefault.store (range.convertTo0to1 (defaultVal));
            rap = dynamic_cast<juce::RangedAudioParameter*>(this);
            jassert (rap != nullptr);
        }
        
        // returns the current default value, within the 0-1 normalized range for this parameter
        float getNormalizedDefault() const { return currentDefault.load(); }
        
        // returns the absolute default value as a float
        float getDefault() const { return AudioParameterFloat::getNormalisableRange().convertFrom0to1 (currentDefault.load()); }
        
        void setDefault (float newDefault)
        {
            currentDefault.store (AudioParameterFloat::getNormalisableRange().convertTo0to1 (newDefault));
        }
        
        // assigns the default value to the parameter's current value
        void refreshDefault()
        {
            currentDefault.store (AudioParameterFloat::getNormalisableRange().convertTo0to1 (this->get()));
        }
        
        // returns the parameter's current value as a normalized float in range 0.0 to 1.0
        float getCurrentNormalizedValue() const { return rap->getValue(); }
        
    private:
        std::atomic<float> currentDefault;
        juce::RangedAudioParameter* rap = nullptr;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatParameter)
    };
    
    
    /*
     Wrapper class around juce::AudioParameterInt that allows you to change its default value at runtime, and exposes the getValue() function as public
     */
    class IntParameter    :     public juce::AudioParameterInt
    {
        using AudioParameterInt = juce::AudioParameterInt;
        
    public:
        // use the constructor just like you would the constructor for juce::AudioParameterInt. All the args are simply forwarded.
        IntParameter(juce::String parameterID, juce::String parameterName, int min, int max, int defaultVal):
        AudioParameterInt(parameterID, parameterName, min, max, defaultVal)
        {
            currentDefault.store (AudioParameterInt::getNormalisableRange().convertTo0to1 (defaultVal));
            rap = dynamic_cast<juce::RangedAudioParameter*>(this);
            jassert (rap != nullptr);
        }
        
        // returns the current default value, within the 0-1 normalized range for this parameter
        float getNormalizedDefault() const { return currentDefault.load(); }
        
        // returns the absolute default value as an int
        int getDefault() const
        {
            return juce::roundToInt (AudioParameterInt::getNormalisableRange().convertFrom0to1 (currentDefault.load()));
        }
        
        void setDefault (int newDefault)
        {
            currentDefault.store (AudioParameterInt::getNormalisableRange().convertTo0to1 (newDefault));
        }
        
        // assigns the default value to the parameter's current value
        void refreshDefault()
        {
            currentDefault.store (AudioParameterInt::getNormalisableRange().convertTo0to1 (this->get()));
        }
        
        // returns the parameter's current value as a normalized float in range 0.0 to 1.0
        float getCurrentNormalizedValue() const { return rap->getValue(); }
        
    private:
        std::atomic<float> currentDefault;
        juce::RangedAudioParameter* rap = nullptr;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntParameter)
    };
    
    
    /*
     Wrapper class around juce::AudioParameterBool that allows you to change its default value at runtime, and exposes the getValue() function as public
     */
    class BoolParameter    :     public juce::AudioParameterBool
    {
        using AudioParameterBool = juce::AudioParameterBool;
        
    public:
        // use the constructor just like you would the constructor for juce::AudioParameterInt. All the args are simply forwarded.
        BoolParameter(juce::String parameterID, juce::String parameterName, bool defaultVal):
        AudioParameterBool(parameterID, parameterName, defaultVal)
        {
            if (defaultVal)
                currentDefault.store (1.0f);
            else
                currentDefault.store (0.0f);
        }
        
        // returns the current default value, within the 0-1 normalized range for this parameter
        float getNormalizedDefault() const { return currentDefault.load(); }
        
        // returns the absolute default value as a bool
        bool getDefault() const
        {
            if (currentDefault.load() == 0.0f)
                return false;
            
            return true;
        }
        
        void setDefault (bool newDefault)
        {
            if (newDefault)
                currentDefault.store (1.0f);
            else
                currentDefault.store (0.0f);
        }
        
        // assigns the default value to the parameter's current value
        void refreshDefault()
        {
            if (this->get())
                currentDefault.store (1.0f);
            else
                currentDefault.store (0.0f);
        }
        
        // returns the parameter's current value as a normalized float in range 0.0 to 1.0
        float getCurrentNormalizedValue() const
        {
            if (this->get())
                return 1.0f;
            
            return 0.0f;
        }
        
    private:
        std::atomic<float> currentDefault;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolParameter)
    };
    
    
}  // namespace bav

