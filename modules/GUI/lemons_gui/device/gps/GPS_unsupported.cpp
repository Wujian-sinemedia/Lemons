#if LEMONS_ENABLE_GPS_LOCATION_SERVICES
#  if (JUCE_MAC || JUCE_IOS || JUCE_ANDROID || JUCE_WINDOWS)
#	error
#  endif
#endif

namespace lemons
{

bool GPSLocation::requestLocationPermissions (bool) { return false; }

bool GPSLocation::isLocationAvailable() { return false; }

GPSLocation GPSLocation::getCurrentLocation() { return {}; }

}  // namespace lemons
