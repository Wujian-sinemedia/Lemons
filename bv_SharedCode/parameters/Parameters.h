

namespace bav
{
    

    class Parameter
    {
        using RangedParam = juce::RangedAudioParameter;
        
    public:
        Parameter (int key, RangedParam* p, float defaultValue,
                   juce::String paramNameShort,
                   juce::String paramNameVerbose)
              : parameterNameShort   (TRANS (paramNameShort)),
                parameterNameVerbose (TRANS (paramNameVerbose)),
                parameterNameVerboseNoSpaces (paramNameVerbose.trim().replaceCharacters (" ", "_")),
                identifier (paramNameVerbose.trim().replaceCharacters (" ", "_")),
                gestureIdentifier (identifier.toString() + "_isChanging"),
                currentDefault (defaultValue), rap (p), keyID (key)
        {
            jassert (rap != nullptr);
        }
        
        virtual ~Parameter() = default;
        
        // returns the current default value, within the 0-1 normalized range for this parameter
        float getNormalizedDefault() const { return currentDefault.load(); }
        
        // assigns the default value to the parameter's current value
        void refreshDefault()
        {
            currentDefault.store (getCurrentNormalizedValue());
        }
        
        // resets the parameter to its currently stored default
        void resetToDefault() { rap->setValueNotifyingHost (currentDefault.load()); }
        
        float getCurrentNormalizedValue()   const { return rap->getValue(); }
        float getCurrentDenormalizedValue() const { return rap->convertFrom0to1 (rap->getValue()); }
        
        // returns a const reference to this parameter's NormalisableRange object
        virtual const juce::NormalisableRange<float>& getRange() const { return rap->getNormalisableRange(); }
        
        // returns a float value normalized in the range 0 to 1, using this parameter's NormalisableRange object
        virtual float normalize (const float input) const { return rap->convertTo0to1(input); }
        
        // takes a normalized float value as input and returns a denormalized float value within the natural range of this parameter.
        virtual float denormalize (const float input) const { return rap->convertFrom0to1(input); }
        
        // exposes the underlying juce::RangedAudioParameter
        RangedParam* orig() const noexcept { return rap; }
        
        int key() const noexcept { return keyID; }
        
        void doAction()
        {
            if (actionableFunction)
                actionableFunction (rap->convertFrom0to1 (rap->getValue()));
        }
        
        std::function<void(float)> actionableFunction = nullptr;
        
        const juce::String parameterNameShort;
        const juce::String parameterNameVerbose;
        const juce::String parameterNameVerboseNoSpaces;
        
        const juce::Identifier identifier;
        const juce::Identifier gestureIdentifier;
        
    protected:
        std::atomic<float> currentDefault;
        
    private:
        RangedParam* const rap;
        const int keyID;
    };
    
    
    
    class FloatParameter  :     public juce::AudioParameterFloat,
                                public bav::Parameter
    {
        using AudioParameterFloat = juce::AudioParameterFloat;
        
    public:
        FloatParameter (int key,
                        juce::String paramNameShort, juce::String paramNameVerbose,
                        juce::NormalisableRange<float> nRange, float defaultVal, juce::String parameterLabel = juce::String(),
                        juce::AudioProcessorParameter::Category parameterCategory = juce::AudioProcessorParameter::genericParameter,
                        std::function<juce::String(float value, int maximumStringLength)> stringFromValue = nullptr,
                        std::function<float(const juce::String& text)> valueFromString = nullptr)
             :  AudioParameterFloat (paramNameVerbose, TRANS (paramNameVerbose), nRange, defaultVal,
                                     parameterLabel, parameterCategory, stringFromValue, valueFromString),
                Parameter (key, this, nRange.convertTo0to1 (defaultVal), paramNameShort, paramNameVerbose)
        {
        }
        
        virtual ~FloatParameter() override = default;
        
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
    

    class IntParameter    :     public juce::AudioParameterInt,
                                public bav::Parameter
    {
        using AudioParameterInt = juce::AudioParameterInt;
        
    public:
        IntParameter (int key,
                      juce::String paramNameShort, juce::String paramNameVerbose,
                      int min, int max, int defaultVal,
                      juce::String parameterLabel = juce::String(),
                      std::function<juce::String(int value, int maximumStringLength)> stringFromInt = nullptr,
                      std::function<int(const juce::String& text)> intFromString = nullptr)
            :   AudioParameterInt (paramNameVerbose, TRANS (paramNameVerbose), min, max, defaultVal,
                                   parameterLabel, stringFromInt, intFromString),
                Parameter (key, this,
                           AudioParameterInt::getNormalisableRange().convertTo0to1 (static_cast<float>(defaultVal)),
                           paramNameShort, paramNameVerbose)
        {
        }
        
        virtual ~IntParameter() override = default;
        
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
    
    

    class BoolParameter    :        public juce::AudioParameterBool,
                                    public bav::Parameter
    {
        using AudioParameterBool = juce::AudioParameterBool;
        
    public:
        BoolParameter (int key,
                       juce::String paramNameShort, juce::String paramNameVerbose,
                       bool defaultVal,
                       juce::String parameterLabel = juce::String(),
                       std::function<juce::String(bool value, int maximumStringLength)> stringFromBool = nullptr,
                       std::function<bool(const juce::String& text)> boolFromString = nullptr)
             :  AudioParameterBool (paramNameVerbose, TRANS (paramNameVerbose), defaultVal, parameterLabel,
                                    stringFromBool, boolFromString),
                Parameter (key, this,
                           AudioParameterBool::getNormalisableRange().convertTo0to1 (static_cast<float>(defaultVal)),
                           paramNameShort, paramNameVerbose)
        {
            setDefault (defaultVal);
        }
        
        virtual ~BoolParameter() override = default;
        
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



    struct MeterParameter :     public FloatParameter
    {
        MeterParameter (int key,
                        juce::String paramNameShort, juce::String paramNameVerbose,
                        juce::NormalisableRange<float> nRange, float defaultVal, juce::String parameterLabel = juce::String(),
                        juce::AudioProcessorParameter::Category parameterCategory = juce::AudioProcessorParameter::genericParameter,
                        std::function<juce::String(float value, int maximumStringLength)> stringFromValue = nullptr,
                        std::function<float(const juce::String& text)> valueFromString = nullptr)
        
        :   FloatParameter (0, paramNameShort, paramNameVerbose, nRange, defaultVal,
                            parameterLabel, parameterCategory, stringFromValue, valueFromString)
        { }
        
        bool isAutomatable() const override final { return false; }
    };


    struct GainMeterParameter : public MeterParameter
    {
        GainMeterParameter (int key,
                            juce::String paramNameShort, juce::String paramNameVerbose,
                            juce::AudioProcessorParameter::Category parameterCategory = juce::AudioProcessorParameter::genericParameter)
        
        :   MeterParameter (key, paramNameShort, paramNameVerbose,
                            juce::NormalisableRange<float>(-60.0f, 0.0f, 0.01f),
                            -60.0f, TRANS ("dB"), parameterCategory,
                            ParameterValueConversionLambdas::gain_stringFromFloat,
                            ParameterValueConversionLambdas::gain_floatFromString)
        { }
    };

    
}  // namespace bav


