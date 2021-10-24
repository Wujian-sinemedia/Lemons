
#pragma once

namespace lemons::gui
{

/** Base class for a kind of component that can be held inside a popup.
    Create a subclass of PopupComponent to hold the content that should dynamically appear/disappear, then create a Popup templated on your custom PopupComponent type to actually hold the window.
    Example usage:
    @code
    using namespace lemons::gui;
 
    // First, create the component that will hold the popup content...
    struct MyPopup : PopupComponent
    {
        using PopupComponent::PopupComponent;
    };
 
    // Now, we can create a holder to actually display our content:
    Popup<MyPopup> popup;
 
    // create/show it like this:
    popup.create ([&]{ popup.destroy(); });
 
    // destroy/hide it like this:
    popup.destroy();
    @endcode
    @see Popup
 */
class PopupComponent : public juce::Component
{
public:
    /** Creates a popup component.
        @param toClose Lambda function that must destroy/close this popup.
     */
	PopupComponent (std::function<void()> toClose);

    /** Closes the popup component, by calling the function passed to the constructor. */
	void close();

private:
	void resized() final;
	bool keyPressed (const juce::KeyPress& key) final;

    /** Called when this component is resized. */
	virtual void resizeTriggered();
    
    /** Called when a keypress is recieved.
        The PopupComponent base class automatically intercepts keypresses, and closes itself if an escape key is pressed; all other keypresses are forwarded to this function.
     */
	virtual bool keyPressRecieved (const juce::KeyPress& key);

	std::function<void()> closeFunc;
};


/** Template class that holds a kind of PopupComponent and can dynamically create and destroy it.
     Example usage:
     @code
     using namespace lemons::gui;
     
     // First, create the component that will hold the popup content...
     struct MyPopup : PopupComponent
     {
        using PopupComponent::PopupComponent;
     };
     
     // Now, we can create a holder to actually display our content:
     Popup<MyPopup> popup;
     
     // create/show it like this:
     popup.create ([&]{ popup.destroy(); });
     
     // destroy/hide it like this:
     popup.destroy();
     @endcode
    @tparam ContentType The type of popup component this holder will display; this type must inherit from PopupComponent.
    @see PopupComponent
 */
template <typename ContentType, LEMONS_MUST_INHERIT_FROM (ContentType, PopupComponent)>
class Popup : public juce::Component
{
public:
    /** Creates a popup component holder.
        At first, the popup is not visible. You must call the create() method to show the popup.
     */
	Popup()
	{
		setInterceptsMouseClicks (false, true);
	}

    /** Resizes the popup. */
	void resized() final
	{
		if (window.get() != nullptr)
			window->setBounds (getLocalBounds());
	}

    /** Returns true if the popup component is currently showing. */
	bool isVisible() const
	{
		return window.get() != nullptr;
	}

    /** Creates the popup component (or recreates it, if it already exists). All constructor arguments are forwarded verbatim to the constructor of ContentType. */
	template <typename... Args>
	void create (Args&&... args)
	{
		window.reset (new ContentType (std::forward<Args> (args)...));
		getTopLevelComponent()->addAndMakeVisible (window.get());
		resized();
	}

    /** Destroys the popup component, if it exists. */
	void destroy()
	{
		if (window.get() != nullptr)
		{
			window.reset();
		}
	}

private:
	std::unique_ptr<ContentType> window;
};

}  // namespace lemons::gui
