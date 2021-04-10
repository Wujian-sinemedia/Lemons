

namespace bav
{
    
    /*
     This function returns a given RangedAudioParameter's current normalized value in the range 0.0f to 1.0f.
     This function is necessary because the provided juce subclasses AudioParameterFloat|Bool|Int implement getValue() as private.
     */
    static inline float getNormalizedParameterValue (juce::RangedAudioParameter& param)
    {
        return param.getValue();
    }
    
    
    
    /*
        My wrapper classes for each individual parameter type all inherit from this base class.
        This base class provides functionality for saving, changing, and recalling a defaut (normalized float) value; as well as some useful functions that each individual parameter type overrides to provide access to data specific to each parameter type through the base class interface.
        This base class does NOT inherit from any of the JUCE parameter classes, in order to avoid a tangle of double- and cross-inheritances with the individual parameter type classes below.
        My typical use case of this base class is to be able to access any parameter value from one function call to my processor, regardless of the type of parameter, e.g.:
     @code
         class MyAudioProcessor  :      public juce::AudioProcessor
         {
         public:
            enum parameterID { gain, bypass };
     
            float getParameterValue (const parameterID id)
            {
                return getParamPtr(id)->getCurrentNormalizedValue();
            }
     
         private:
             bav::Parameter* getParamPtr (const parameterID id)
             {
                 switch (id)
                 {
                    case (gain):   return gainParam;
                    case (bypass): return bypassParam;
                 }
             }
     
             bav::FloatParameter* gainParam;
             bav::BoolParameter* bypassParam;
            //NB. you can initialize these by doing e.g. bypassParam = dynamic_cast<bav::BoolParameter*>(apvts.getParameter("bypass"));
         };
     @end-code
    */
    
    class Parameter
    {
        using RangedParam = juce::RangedAudioParameter;
        
    public:
        Parameter(RangedParam* p, float defaultValue): currentDefault(defaultValue), rap(p)
        {
            jassert (rap != nullptr);
        }
        
        ~Parameter() = default;
        
        // returns the current default value, within the 0-1 normalized range for this parameter
        float getNormalizedDefault() const { return currentDefault.load(); }
        
        // assigns the default value to the parameter's current value
        void refreshDefault()
        {
            currentDefault.store (getCurrentNormalizedValue());
        }
        
        // returns the parameter's current value as a normalized float in range 0.0 to 1.0
        float getCurrentNormalizedValue() const { return rap->getValue(); }
        
        // returns a const reference to this parameter's NormalisableRange object
        virtual const juce::NormalisableRange<float>& getRange() const { return rap->getNormalisableRange(); }
        
        // returns a float value normalized in the range 0 to 1, using this parameter's NormalisableRange object
        virtual float normalize (const float input) const { return rap->convertTo0to1(input); }
        
        // takes a normalized float value as input and returns a denormalized float value within the natural range of this parameter.
        virtual float denormalize (const float input) const { return rap->convertFrom0to1(input); }
        
        // value passed should be normalized
        void changeValue (float newValue) { rap->setValue(newValue); }
        
        // value passed should be normalized
        void changeValueNotifyHost (float newValue) { rap->setValueNotifyingHost(newValue); }
        
    protected:
        std::atomic<float> currentDefault;
        
    private:
        RangedParam* const rap = nullptr;
    };
    
    
    
    /*
     Wrapper class around juce::AudioParameterFloat that allows you to change its default value at runtime, and exposes the getValue() function as public
     */
    class FloatParameter  :     public juce::AudioParameterFloat,
                                public bav::Parameter
    {
        using AudioParameterFloat = juce::AudioParameterFloat;
        
    public:
        // use the constructor just like you would the constructor for juce::AudioParameterFloat. All the args are simply forwarded.
        FloatParameter(juce::String parameterID, juce::String parameterName, juce::NormalisableRange<float> range, float defaultVal):
                AudioParameterFloat(parameterID, parameterName, range, defaultVal),
                Parameter(dynamic_cast<juce::RangedAudioParameter*>(this), range.convertTo0to1 (defaultVal))
        {
        }
        
        // returns the absolute default value as a float
        float getDefault() const { return AudioParameterFloat::getNormalisableRange().convertFrom0to1 (currentDefault.load()); }
        
        void setDefault (float newDefault)
        {
            currentDefault.store (AudioParameterFloat::getNormalisableRange().convertTo0to1 (newDefault));
        }
        
        // returns a const reference to this parameter's NormalisableRange object
        const juce::NormalisableRange<float>& getRange() const override { return AudioParameterFloat::getNormalisableRange(); }
        
        // returns a float value normalized in the range 0 to 1, using this parameter's NormalisableRange object
        float normalize (const float input) const override { return AudioParameterFloat::convertTo0to1(input); }
        
        // takes a normalized float value as input and returns a denormalized float value within the natural range of this parameter.
        float denormalize (const float input) const override { return AudioParameterFloat::convertFrom0to1(input); }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatParameter)
    };
    
    
    /*
     Wrapper class around juce::AudioParameterInt that allows you to change its default value at runtime, and exposes the getValue() function as public
     */
    class IntParameter    :     public juce::AudioParameterInt,
                                public bav::Parameter
    {
        using AudioParameterInt = juce::AudioParameterInt;
        
    public:
        // use the constructor just like you would the constructor for juce::AudioParameterInt. All the args are simply forwarded.
        IntParameter(juce::String parameterID, juce::String parameterName, int min, int max, int defaultVal):
                AudioParameterInt(parameterID, parameterName, min, max, defaultVal),
                Parameter(dynamic_cast<juce::RangedAudioParameter*>(this),
                          AudioParameterInt::getNormalisableRange().convertTo0to1 (float(defaultVal)))
        {
        }
        
        // returns the absolute default value as an int
        int getDefault() const
        {
            return juce::roundToInt (AudioParameterInt::getNormalisableRange().convertFrom0to1 (currentDefault.load()));
        }
        
        void setDefault (int newDefault)
        {
            currentDefault.store (AudioParameterInt::getNormalisableRange().convertTo0to1 (float(newDefault)));
        }
        
        // returns a const reference to this parameter's NormalisableRange object
        const juce::NormalisableRange<float>& getRange() const override { return AudioParameterInt::getNormalisableRange(); }
        
        // returns a float value normalized in the range 0 to 1, using this parameter's NormalisableRange object
        float normalize (const float input) const override { return AudioParameterInt::convertTo0to1(input); }
        
        // takes a normalized float value as input and returns a denormalized float value within the natural range of this parameter.
        float denormalize (const float input) const override { return AudioParameterInt::convertFrom0to1(input); }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntParameter)
    };
    
    
    /*
     Wrapper class around juce::AudioParameterBool that allows you to change its default value at runtime, and exposes the getValue() function as public
     */
    class BoolParameter    :        public juce::AudioParameterBool,
                                    public bav::Parameter
    {
        using AudioParameterBool = juce::AudioParameterBool;
        
    public:
        // use the constructor just like you would the constructor for juce::AudioParameterInt. All the args are simply forwarded.
        BoolParameter(juce::String parameterID, juce::String parameterName, bool defaultVal):
                AudioParameterBool(parameterID, parameterName, defaultVal),
                Parameter(dynamic_cast<juce::RangedAudioParameter*>(this),
                          AudioParameterBool::getNormalisableRange().convertTo0to1 (float(defaultVal)))
        {
            setDefault (defaultVal);
        }
        
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
        
        // returns a const reference to this parameter's NormalisableRange object
        const juce::NormalisableRange<float>& getRange() const override { return AudioParameterBool::getNormalisableRange(); }
        
        // returns a float value normalized in the range 0 to 1, using this parameter's NormalisableRange object
        float normalize (const float input) const override { return AudioParameterBool::convertTo0to1(input); }
        
        // takes a normalized float value as input and returns a denormalized float value within the natural range of this parameter.
        float denormalize (const float input) const override { return AudioParameterBool::convertFrom0to1(input); }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolParameter)
    };

    
}  // namespace bav



