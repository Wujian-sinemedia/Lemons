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

namespace lemons
{

DeviceMotion::Listener::Listener (DeviceMotion& manager)
    : m (manager)
{
	m.addListener (*this);
}

DeviceMotion::Listener::~Listener()
{
	m.removeListener (*this);
}

/*-----------------------------------------------------------------------------------*/

void DeviceMotion::addListener (Listener& l)
{
	listeners.add (&l);
}

void DeviceMotion::removeListener (Listener& l)
{
	listeners.remove (&l);
}

DeviceMotion::Coords DeviceMotion::getAcceleration() const noexcept { return acceleration; }

DeviceMotion::Coords DeviceMotion::getGravity() const noexcept { return gravity; }

DeviceMotion::Coords DeviceMotion::getRotation() const noexcept { return rotation; }

DeviceMotion::Coords DeviceMotion::getAttitude() const noexcept { return attitude; }

void DeviceMotion::accelerationChanged (double x, double y, double z)
{
	acceleration.x = x;
	acceleration.y = y;
	acceleration.z = z;

	listeners.call ([&] (Listener& l)
	                { l.accelerationChanged (acceleration); });
}

void DeviceMotion::gravityChanged (double x, double y, double z)
{
	gravity.x = x;
	gravity.y = y;
	gravity.z = z;

	listeners.call ([&] (Listener& l)
	                { l.gravityChanged (gravity); });
}

void DeviceMotion::rotationChanged (double x, double y, double z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;

	listeners.call ([&] (Listener& l)
	                { l.rotationChanged (rotation); });
}

void DeviceMotion::attitudeChanged (double x, double y, double z)
{
	attitude.x = x;
	attitude.y = y;
	attitude.z = z;

	listeners.call ([&] (Listener& l)
	                { l.attitudeChanged (attitude); });
}

#if ! JUCE_IOS

DeviceMotion::DeviceMotion()
{
}

DeviceMotion::~DeviceMotion()
{
}

#  if ! JUCE_ANDROID

void DeviceMotion::start()
{
	if (running)
		return;

	running = true;
}

void DeviceMotion::stop()
{
	if (! running)
		return;

	running = false;
}

#  endif

#endif


bool DeviceMotion::isRunning() const noexcept
{
#if (JUCE_IOS || JUCE_ANDROID)
	return running;
#else
	return false;
#endif
}

}  // namespace lemons
