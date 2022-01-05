
#pragma once

#include <lemons_core/lemons_core.h>

namespace lemons::gui::components
{

/** @addtogroup lemons_popup_component Popup components
    @ingroup lemons_gui_components
    Base classes for working with popup components.
 */
///@{

/** Base class for a kind of component that can be displayed as a popup.
    Create a subclass of PopupComponentBase to hold the content that should dynamically appear/disappear, then create a Popup templated on your custom PopupComponent type to actually hold the window.
    Example usage:
    @code
    using namespace lemons::gui;

    // First, create the component that will hold the popup content...
    struct MyPopup : PopupComponentBase
    {
        using PopupComponentBase::PopupComponentBase;
    };

    // Now, we can create a holder to actually display our content:
    Popup<MyPopup> popup;

    // create/show it like this:
    popup.create ([&]{ popup.destroy(); }, other args...);

    // destroy/hide it like this:
    popup.destroy();
    @endcode
    @see Popup, PopupComponent
 */
class PopupComponentBase : public Component
{
public:
	/** Creates a popup component.
	    @param toClose Lambda function that must destroy/close this popup.
	    @param useCloseButton If true, this component will create a "close" button in the upper left corner of the owned content that, when clicked, will destroy this component.
	    @param escapeKeyCloses If true, this component consumes all escape key presses while it's open, and an escape key press will destroy this component.
	 */
	explicit PopupComponentBase (std::function<void()> toClose, bool useCloseButton = true, bool escapeKeyCloses = true);

	/** Creates a popup component with a closing lambda that calls reset() on the unique_ptr you pass here. This constructor is intended as shorthand for this common design pattern:
	    @code
	    std::unique_pointer<PopupComponentBase> comp;

	    comp.reset (new PopupComponentBase ([](){ comp.reset() }));
	    @endcode
	    so instead, you can use this constructor like so:
	    @code
	    std::unique_pointer<PopupComponentBase> comp;

	    comp.reset (new PopupComponentBase (comp));
	    @endcode
	 @param holder A unique pointer holding this component
	 */
	explicit PopupComponentBase (std::unique_ptr<PopupComponentBase>& holder, bool useCloseButton = true, bool escapeKeyCloses = true);

	/** Destructor. */
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


/** A kind of PopupComponentBase that owns and displays a kind of Component.
    If you use this class, you never have to specialize PopupComponentBase to create popup windows, you can just write them as regular Components and then stick them in one of these.
    @tparam OwnedContentType The type of component this popup will display. This type must inherit from juce::Component.
    @see PopupComponentBase, Popup
 */
template <typename OwnedContentType, LEMONS_MUST_INHERIT_FROM (OwnedContentType, Component)>
class PopupComponent final : public PopupComponentBase
{
public:
	/** Creates a PopupComponent with a lambda to destroy itself.
	    @param toClose Lambda function that must destroy this PopupComponent.
	    @param useCloseButton  If true, the wrapper component will create a "close" button in the upper left corner of the owned content that, when clicked, will destroy the popup.
	    @param escapeKeyCloses If true, the wrapper component consumes all escape key presses, and an escape key press will destroy the popup.
	    @param args Optional additional arguments that will be forwarded to the constuctor of OwnedContentType.
	 */
	template <typename... Args>
	explicit PopupComponent (std::function<void()> toClose, bool useCloseButton, bool escapeKeyCloses, Args&&... args)
	    : PopupComponentBase (toClose, useCloseButton, escapeKeyCloses)
	    , content (std::forward<Args> (args)...)
	{
		addAndMakeVisible (content);
	}

	/** Creates a PopupComponent whose destruction lambda calls reset on the unique_ptr it lives in. See the constructors for PopupComponentBase for more detailed docs.
	    @param holder unique_ptr holding this PopupComponent.
	    @param useCloseButton  If true, the wrapper component will create a "close" button in the upper left corner of the owned content that, when clicked, will destroy the popup.
	    @param escapeKeyCloses If true, the wrapper component consumes all escape key presses, and an escape key press will destroy the popup.
	    @param args Optional additional arguments that will be forwarded to the constuctor of OwnedContentType.
	 */
	template <typename... Args>
	explicit PopupComponent (std::unique_ptr<PopupComponentBase>& holder, bool useCloseButton, bool escapeKeyCloses, Args&&... args)
	    : PopupComponentBase (holder, useCloseButton, escapeKeyCloses)
	    , content (std::forward<Args> (args)...)
	{
		addAndMakeVisible (content);
	}

	/** The owned content component. */
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


/** A class that owns and displays a kind of component as a popup, and can dynamically create and destroy it.
    This class is itself a component, so you can add it to whatever parent component it lives in using @code addAndMakeVisible() @endcode
     Example usage:
     @code
     using namespace lemons::gui;

     // First, create the component that will hold the popup content...
     struct MyPopup : Component
     {
        // cool stuff here...
     };

     // Now, we can create a holder to actually display our content:
     Popup<MyPopup> popup;

     // create/show it like this:
     popup.create (your type's constructor args...);

     // destroy/hide it like this:
     popup.destroy();
     @endcode
    @tparam ContentType The type of popup component this holder will display; this type must inherit from Component.
    @see PopupComponent, PopupComponentBase
 */
template <typename ContentType, LEMONS_MUST_INHERIT_FROM (ContentType, Component)>
class Popup final : public Component
{
	using Content = PopupComponent<ContentType>;

public:
	/** Creates a popup component holder.
	    The close button and escape key options are constant for the life of this object, so you only have to set them once on creation.
	    At first, the popup is not visible. You must call the create() method to show the popup.
	    @param useCloseButton  If true, the wrapper component will create a "close" button in the upper left corner of the owned content that, when clicked, will destroy the popup.
	    @param escapeKeyCloses If true, the wrapper component consumes all escape key presses, and an escape key press will destroy the popup.
	 */
	explicit Popup (bool useCloseButton = true, bool escapeKeyCloses = true)
	    : wrapperCloseButton (useCloseButton)
	    , escapeKeyClosesWrapper (escapeKeyCloses)
	{
		setInterceptsMouseClicks (false, true);
	}

	/** Returns true if the popup component is currently showing. */
	[[nodiscard]] bool isVisible() const
	{
		return window.get() != nullptr;
	}

	/** Creates the popup component (or recreates it, if it already exists). All constructor arguments are forwarded verbatim to the constructor of ContentType.
	    @returns Safe pointer to the new owned component.
	 */
	template <typename... Args>
	juce::Component::SafePointer<ContentType> create (Args&&... args)
	{
		window.reset (new Content (window, wrapperCloseButton, escapeKeyClosesWrapper,
		                           std::forward<Args> (args)...));

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
		if (window.get() == nullptr)
			return false;

		window.reset();
		return true;
	}

private:
	void resized() final
	{
		if (auto* w = window.get())
			w->setBounds (getLocalBounds());
	}

	bool wrapperCloseButton { true }, escapeKeyClosesWrapper { true };

	std::unique_ptr<Content> window;
};

///@}

}  // namespace lemons::gui::components
