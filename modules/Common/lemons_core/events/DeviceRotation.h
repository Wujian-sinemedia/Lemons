#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


namespace lemons
{

/** Use this struct's static methods to control various properties of the device's screen rotation/orientation.
 */
struct DeviceRotation final
{
public:
	using Orientation = juce::Desktop::DisplayOrientation;

	/** RAII mechanism for temporarily disabling device rotation.
	    When this object's destructor is called, it restores the rotations that were enabled when the object was constructed.
	 */
	struct ScopedDisabler final
	{
		/** Constructor. */
		explicit ScopedDisabler();

		/** Destructor. */
		~ScopedDisabler();

	private:
		const int prevOrientations;
	};

	/** Use this struct to register a lambda that will be called any time the device's screen orientation changes.
	 */
	struct Listener : private juce::Timer
	{
		using Callback = std::function<void (Orientation)>;

		/** Constructor. */
		explicit Listener (Callback&& callbackToUse);

		/** Destructor. */
		virtual ~Listener() override;

	private:
		void timerCallback() final;

		Callback    callback;
		Orientation prevOrientation;
	};

	/** This method attempts to disable screen rotation by only allowing the current orientation. */
	static void disable();

	/** This method attempts to enable only vertical screen orientations. */
	static void enableOnlyVertical();

	/** This method attempts to enable only landscape screen orientations. */
	static void enableOnlyLandscape();

	/** This method returns the current orientation of the device's screen. */
	[[nodiscard]] static Orientation getCurrent();

private:
	using Desktop = juce::Desktop;
};

}  // namespace lemons
