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

#if JUCE_IOS
#  include <CoreMotion/CoreMotion.h>
#endif

namespace lemons
{

#if JUCE_IOS

/* iOS implementation of a watcher object */

@interface MotionManagerWrapper : NSObject
{
	MotionManager* owner;
}

@property (strong, nonatomic) CMMotionManager* motionManager;

@end

@implementation MotionManagerWrapper

- (void)startMotionManager
{
	self.motionManager                            = [[CMMotionManager alloc] init];
	self.motionManager.deviceMotionUpdateInterval = 1.0 / 60.0;
	[self.motionManager startDeviceMotionUpdatesToQueue:[NSOperationQueue currentQueue]
	                                        withHandler:^(CMDeviceMotion* motion, NSError* error) {
		                                      [self outputUserAcceleration:motion.userAcceleration];
		                                      [self outputGravity:motion.gravity];
		                                      [self outputRotationRate:motion.rotationRate];
		                                      [self outputAttitude:motion.attitude];
	                                        }];
}


- (void)stopMotionManager
{
	[self.motionManager stopGyroUpdates];
	[self.motionManager stopAccelerometerUpdates];
	self.motionManager = nil;
}


- (void)outputUserAcceleration:(CMAcceleration)acceleration
{
	owner->accelerationChanged (acceleration.x, acceleration.y, acceleration.z);
}


- (void)outputGravity:(CMAcceleration)gravity
{
	owner->gravityChanged (gravity.x, gravity.y, gravity.z);
}


- (void)outputRotationRate:(CMRotationRate)rotation
{
	owner->rotationChanged (rotation.x, rotation.y, rotation.z);
}


- (void)outputAttitude:(CMAttitude*)attitude
{
	owner->attitudeChanged (attitude.pitch, attitude.roll, attitude.yaw);
}


- (id)initWithOwner:(MotionManager*)owner_
{
	if ((self = [super init]) != nil) { owner = owner_; };
	return self;
}


- (void)dealloc
{
	[self stopMotionManager];
	[super dealloc];
}

@end

#endif /* if JUCE_IOS */

/*-----------------------------------------------------------------------------------*/

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

DeviceMotion::DeviceMotion()
{
#if JUCE_IOS
	MotionManagerWrapper* newManager = [[MotionManagerWrapper alloc] initWithOwner:this];
	[newManager retain];
	motionManagerWrapper = newManager;
#endif
}

DeviceMotion::~DeviceMotion()
{
#if JUCE_IOS
	[((MotionManagerWrapper*) motionManagerWrapper) release];
#endif
}

void DeviceMotion::start()
{
	if (running)
		return;

	running = true;

#if JUCE_IOS

	[(MotionManagerWrapper*) motionManagerWrapper startMotionManager];

#elif JUCE_ANDROID

	ASensorEventQueue_enableSensor (motionEventQueue, accelerometer);
	ASensorEventQueue_setEventRate (motionEventQueue,
	                                accelerometer,
	                                SENSOR_REFRESH_PERIOD_US);
	ASensorEventQueue_enableSensor (motionEventQueue, rotation);
	ASensorEventQueue_setEventRate (motionEventQueue,
	                                rotation,
	                                SENSOR_REFRESH_PERIOD_US);
	ASensorEventQueue_enableSensor (motionEventQueue, gravity);
	ASensorEventQueue_setEventRate (motionEventQueue,
	                                gravity,
	                                SENSOR_REFRESH_PERIOD_US);
	ASensorEventQueue_enableSensor (motionEventQueue, attitude);
	ASensorEventQueue_setEventRate (motionEventQueue,
	                                attitude,
	                                SENSOR_REFRESH_PERIOD_US);

	t.startTimerHz (SENSOR_REFRESH_RATE_HZ);

#endif
}

void DeviceMotion::stop()
{
	if (! running)
		return;

	running = false;

#if JUCE_IOS

	[(MotionManagerWrapper*) motionManagerWrapper stopMotionManager];

#elif JUCE_ANDROID

	ASensorEventQueue_disableSensor (motionEventQueue, accelerometer);
	ASensorEventQueue_disableSensor (motionEventQueue, rotation);
	ASensorEventQueue_disableSensor (motionEventQueue, gravity);
	ASensorEventQueue_disableSensor (motionEventQueue, attitude);

	t.stopTimer();

#endif
}

bool DeviceMotion::isRunning() const noexcept
{
#if (JUCE_IOS || JUCE_ANDROID)
	return running;
#else
	return false;
#endif
}

#if JUCE_ANDROID
void MotionManager::update()
{
	ALooper_pollAll (0, nullptr, nullptr, nullptr);
	ASensorEvent event;

	while (ASensorEventQueue_getEvents (motionEventQueue, &event, 1) > 0)
	{
		if (event.type == ASENSOR_TYPE_LINEAR_ACCELERATION)
		{
			accelerationChanged (event.acceleration.x * -0.08,
			                     event.acceleration.y * 0.08,
			                     event.acceleration.z * 0.08);
		}
		else if (event.type == ASENSOR_TYPE_GYROSCOPE)
		{
			rotationChanged (event.vector.x * 1.0,
			                 event.vector.y * 1.0,
			                 event.vector.z * 1.0);
		}
		else if (event.type == ASENSOR_TYPE_GRAVITY)
		{
			gravityChanged (event.acceleration.x * -0.1,
			                event.acceleration.y * 0.1,
			                event.acceleration.z * 0.1);
		}
		else if (event.type == ASENSOR_TYPE_GAME_ROTATION_VECTOR)
		{
			rotationChanged (event.vector.y * 2.0,
			                 event.vector.x * 2.0,
			                 event.vector.z * 2.0);
		}
	}
}
#endif

}  // namespace lemons
