
namespace bav
{

struct SystemInitializer
{
    SystemInitializer (juce::File translationFileToUse = juce::File())
    {
#if BV_USE_NE10
        ne10_init();
#endif
        
        if (translationFileToUse.existsAsFile())
            initializeTranslations (translationFileToUse);
    }
    
    virtual ~SystemInitializer()
    { }
};



struct GUIInitializer
{
#if ! JUCE_OPENGL
    GUIInitializer()
    { }
#endif
    
    
    GUIInitializer (juce::Component* topLevelComponent = nullptr)
    {
#if JUCE_OPENGL
        jassert (topLevelComponent != nullptr);
        openGLContext.attachTo (*topLevelComponent);
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

};
