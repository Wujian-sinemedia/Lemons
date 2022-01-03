
#pragma once

#include <lemons_core/lemons_core.h>

namespace lemons::gui
{

using juce::Component;

/** @name lemons_popup_component Popup
    @ingroup lemons_gui_components
    Base class for a popup component.
 */
///@{

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
class PopupComponentBase : public Component
{
public:
	/** Creates a popup component.
	    @param toClose Lambda function that must destroy/close this popup.
	 */
	explicit PopupComponentBase (std::function<void()> toClose, bool useCloseButton = true, bool escapeKeyCloses = true);

	/** Creates a closing lambda that calls reset() on the unique_ptr you pass here.
	 */
	explicit PopupComponentBase (std::unique_ptr<PopupComponentBase>& holder, bool useCloseButton = true, bool escapeKeyCloses = true);

	virtual ~PopupComponentBase() = default;

	/** Closes the popup component, by calling the function passed to the constructor. */
	void close() const;

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

	bool escapeKeyDestroys { true };

	std::unique_ptr<TextButton> closeButton;
};


template <typename OwnedContentType, LEMONS_MUST_INHERIT_FROM (OwnedContentType, Component)>
class PopupComponent final : public PopupComponentBase
{
public:
	template <typename... Args>
	explicit PopupComponent (std::function<void()> toClose, bool useCloseButton, bool escapeKeyCloses, Args&&... args)
	    : PopupComponentBase (toClose, useCloseButton, escapeKeyCloses)
	    , content (std::forward<Args> (args)...)
	{
		addAndMakeVisible (content);
	}

	template <typename... Args>
	explicit PopupComponent (std::unique_ptr<PopupComponentBase>& holder, bool useCloseButton, bool escapeKeyCloses, Args&&... args)
	    : PopupComponentBase (holder, useCloseButton, escapeKeyCloses)
	    , content (std::forward<Args> (args)...)
	{
		addAndMakeVisible (content);
	}

	OwnedContentType content;

private:
	void resizeTriggered() final
	{
		content.setBounds (getLocalBounds());
	}

	bool keyPressRecieved (const juce::KeyPress& key) final
	{
		return content.keyPressed (key);
	}
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
template <typename ContentType, LEMONS_MUST_INHERIT_FROM (ContentType, Component)>
class Popup : public Component
{
public:
	/** Creates a popup component holder.
	    At first, the popup is not visible. You must call the create() method to show the popup.
	 */
	explicit Popup()
	{
		setInterceptsMouseClicks (false, true);
	}

	/** Returns true if the popup component is currently showing. */
	[[nodiscard]] bool isVisible() const
	{
		return window.get() != nullptr;
	}

	/** Creates the popup component (or recreates it, if it already exists). All constructor arguments are forwarded verbatim to the constructor of ContentType. */
	template <typename... Args>
	juce::Component::SafePointer<ContentType> create (Args&&... args)
	{
		window.reset (new PopupComponent<ContentType> (window, true, true, std::forward<Args> (args)...));

		auto* ptr = window.get();
		getTopLevelComponent()->addAndMakeVisible (ptr);
		resized();

		return { ptr->content };
	}

	/** Destroys the popup component, if it exists.
	 @returns True if the component existed before this function call.
	 */
	bool destroy()
	{
		if (window.get() != nullptr)
		{
			window.reset();
			return true;
		}

		return false;
	}

private:
	void resized() final
	{
		if (window.get() != nullptr)
			window->setBounds (getLocalBounds());
	}

	std::unique_ptr<PopupComponent<ContentType>> window;
};

///@}

}  // namespace lemons::gui
