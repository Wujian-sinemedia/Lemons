
#pragma once

namespace bav::gui
{

template<class ContentType>
class PopupComponent : public juce::Component
{
public:
    template<typename... Args>
    PopupComponent (std::function<void()> toClose, Args&&... args)
    : closeFunc (toClose), content(std::forward<Args>(args)...)
    {
        setWantsKeyboardFocus (true);
    }
    
private:
    void resized() final
    {
        grabKeyboardFocus();
        content.setBounds (getLocalBounds());
    }
    
    bool keyPressed (const juce::KeyPress& key) final
    {
        if (key.getKeyCode() == juce::KeyPress::escapeKey)
        {
            closeFunc();
            return true;
        }
        
        return false;
    }
    
    std::function<void()> closeFunc;
    
    ContentType content;
};


template<typename ContentType>
class Popup : public juce::Component
{
public:
    Popup()
    {
        setInterceptsMouseClicks (false, true);
    }
    
    void resized() final
    {
        if (auto* c = window.get())
            c->setBounds (getLocalBounds());
    }
    
    bool isVisible() const
    {
        return window.get() != nullptr;
    }
    
    template<typename... Args>
    void create (Args&&... args)
    {
        window.reset (new PopupComponent<ContentType>([&]{ destroy(); }, std::forward<Args>(args)...));
        getTopLevelComponent()->addAndMakeVisible (window);
        resized();
    }
    
    void destroy()
    {
        window.reset();
    }
    
private:
    std::unique_ptr<PopupComponent<ContentType>> window;
};

}
