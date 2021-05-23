
namespace bav
{

struct SystemInitializer
{
    SystemInitializer()
    {
#if BV_USE_NE10
        ne10_init();
#endif
        
#if BV_HAS_BINARY_DATA
        initializeTranslationsFromBinaryData (getDefaultTranslationFile());
#endif
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
        
        AutoLock::setEnabled (false);
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
