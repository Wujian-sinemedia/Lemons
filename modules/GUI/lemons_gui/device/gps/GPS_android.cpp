#if ! JUCE_ANDROID
#  error
#endif

#if ! LEMONS_ENABLE_GPS_LOCATION_SERVICES
#  error
#endif

namespace lemons
{

bool GPSLocation::requestLocationPermissions (bool coarseLocation)
{
	jassertfalse;  // TODO
	return false;
}

bool GPSLocation::isLocationAvailable()
{
	auto* env = getEnv();

	static jmethodID _isLocationAvailable = nullptr;
	if (_isLocationAvailable == nullptr)
		_isLocationAvailable = env->GetMethodID (GetJNIActivityClass(), "_isLocationAvailable", "()Z");

	if (_isLocationAvailable != nullptr)
		return env->CallBooleanMethod (cachedActivity, _isLocationAvailable);

	return false;
}

GPSLocation GPSLocation::getCurrentLocation()
{
	auto* env = getEnv();
	if (env == nullptr)
		return {};

	static jmethodID _getGPSLocation = nullptr;
	if (_getGPSLocation == nullptr)
		_getGPSLocation = env->GetMethodID (GetJNIActivityClass(), "_getGPSLocation", "()[D");

	if (_getGPSLocation == nullptr)
		return {};  // Possibly unimplemented or unsupported method...

	auto javaGPSCurrentLocationResult = (jdoubleArray) env->CallObjectMethod (cachedActivity, _getGPSLocation);
	if (javaGPSCurrentLocationResult == nullptr)
		return {};  // If this is reached, the GPS permission was probably disabled but it's hard to say exactly...

	auto                numItems = env->GetArrayLength (javaGPSCurrentLocationResult);
	std::vector<double> input (numItems);
	env->GetDoubleArrayRegion (javaGPSCurrentLocationResult, 0, numItems, &input[0]);

	GPSLocation         result;
	decltype (numItems) index = 0;
	if (index < numItems) result.latitude = (double) input[index++];
	if (index < numItems) result.longitude = (double) input[index++];
	if (index < numItems) result.altitude = (double) input[index++];

	return result;
}

}  // namespace lemons
