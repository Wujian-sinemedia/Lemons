
namespace bav::dsp::osc
{
    
    template<typename SampleType>
    class Sine
    {
    public:
        Sine() { }
        virtual ~Sine() = default;
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sine)
    };
    
    
    template<typename SampleType>
    class Saw
    {
    public:
        Saw() { }
        virtual ~Saw() = default;
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Saw)
    };
    
    
    template<typename SampleType>
    class Square
    {
    public:
        Square() { }
        virtual ~Square() = default;
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Square)
    };
    
    
    template<typename SampleType>
    class Triangle
    {
    public:
        Triangle() { }
        virtual ~Triangle() = default;
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Triangle)
    };
    
    
}  // namespace
