#pragma once

namespace bav::gui
{

struct GUIInitializer
{
    GUIInitializer (juce::Component& componentToUse)
    {
#if JUCE_OPENGL
        openGLContext.attachTo (componentToUse);
#else
        juce::ignoreUnused (componentToUse);
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
