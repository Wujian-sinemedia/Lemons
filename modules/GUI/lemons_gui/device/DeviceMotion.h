/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */

#pragma once

#if JUCE_ANDROID
#  include <android/sensor.h>
#endif

namespace lemons
{

/** @ingroup lemons_device
    Use this class to report various aspects of the device's motion, when sensor data is available, and register listeners.
 */
class DeviceMotion final
{
public:

	/** Simple X, Y, and Z coordinates used in all the kinds of motion data. */
	struct Coords
	{
		double x { 0. };
		double y { 0. };
		double z { 0. };
	};

	/** A listener that recieves callbacks with each kind of motion data. */
	struct Listener
	{
		explicit Listener (DeviceMotion& manager);

		virtual ~Listener();

		virtual void accelerationChanged (Coords) { }
		virtual void gravityChanged (Coords) { }
		virtual void rotationChanged (Coords) { }
		virtual void attitudeChanged (Coords) { }

	private:
		DeviceMotion& m;
	};

	/** Constructor. */
	explicit DeviceMotion();

	/** Destructor. */
	~DeviceMotion();

	/** Starts the motion manager's tracking of the device's sensor data. */
	void start();

	/** Stops the motion manager's tracking of the device's sensor data. */
	void stop();

	/** Returns true if the motion manager is currently tracking the device's sensor data. Note that this always returns false on desktop platforms. */
	[[nodiscard]] bool isRunning() const noexcept;

	/** Returns the latest measured acceleration data. */
	[[nodiscard]] Coords getAcceleration() const noexcept;

	/** Returns the latest measured gravity data. */
	[[nodiscard]] Coords getGravity() const noexcept;

	/** Returns the latest measured rotation data. */
	[[nodiscard]] Coords getRotation() const noexcept;

	/** Returns the latest measured attitude data. */
	[[nodiscard]] Coords getAttitude() const noexcept;

private:
	void addListener (Listener& l);
	void removeListener (Listener& l);

	void accelerationChanged (double x, double y, double z);
	void gravityChanged (double x, double y, double z);
	void rotationChanged (double x, double y, double z);
	void attitudeChanged (double x, double y, double z);

	bool running { false };

	Coords acceleration, gravity, rotation, attitude;

	juce::ListenerList<Listener> listeners;

#if JUCE_IOS

	void* motionManagerWrapper;

#elif JUCE_ANDROID

	void update();

	constexpr int LOOPER_ID_USER           = 3;
	constexpr int SENSOR_REFRESH_RATE_HZ   = 100;
	constexpr int SENSOR_REFRESH_PERIOD_US = 1000000 / SENSOR_REFRESH_RATE_HZ;

	ASensorManager*    sensorManager;
	ASensorEventQueue* motionEventQueue;
	ALooper*           looper;

	const ASensor* accelerometer;
	const ASensor* rotation;
	const ASensor* gravity;
	const ASensor* attitude;

	TimerCallback t { [&]
		              { if (running) update(); },
		              SENSOR_REFRESH_RATE_HZ, false };
#endif
};

}  // namespace lemons
