
#pragma once

namespace bav
{

class ApplicationBase  :    private GUIInitializer,
                            private SystemInitializer
{
public:
    ApplicationBase() { }
    
    virtual ~ApplicationBase() override { }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationBase)
};

}  // namespace
