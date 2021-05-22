
namespace bav
{

struct SystemInitializer
{
    SystemInitializer()
    {
#if BV_USE_NE10
        ne10_init();
#endif
        
        initializeTranslationsFromBinaryData (getDefaultTranslationFile());
    }
    
    virtual ~SystemInitializer()
    { }
};



struct GUIInitializer
{
    GUIInitializer (juce::Component* topLevelComponent = nullptr)
    {
#if JUCE_OPENGL
        jassert (topLevelComponent != nullptr);
        openGLContext.attachTo (*topLevelComponent);
#else
        juce::ignoreUnused (topLevelComponent);
#endif
    }
    
    virtual ~GUIInitializer()
    {
#if JUCE_OPENGL
        openGLContext.detach();
#endif
    }
    
    
private:
#if JUCE_OPENGL
    OpenGLContext openGLContext;
#endif
};

}  // namespace
