#pragma once

#include <juce_core/juce_core.h>

namespace lemons
{

//==============================================================================
/** Represents a GPS location that provides the coordinates and altitude.
    Call getCurrentLocation to get the platform's GPS location.
*/
class GPSLocation final
{
public:
	/** Represents a null GPS location.
	    The latitude and longitude will be infinity.
	    @see isNull
	*/
	GPSLocation();

	/** Creates a GPS location based on the provided coordinates (in degrees), and the altitude. */
	explicit GPSLocation (double latitude, double longitude, double altitude = 0.);

	/** Copy constructor. */
	GPSLocation (const GPSLocation& other);

	/** Destructor. */
	~GPSLocation() = default;

	/** @returns true if the operation succeeded and that the user accepted.
	    This will return false if the platform doesn't have location support or the user didn't want to give access.
	    @coarseLocation This is only applicable to non-Apple platforms.
	                    If false, this requests fine location handling.
	*/
	static bool requestLocationPermissions (bool coarseLocation);

	/** @returns true if the location can be retrieved.
	    This can return false if the platform doesn't have location support or has location disabled.
	    @see requestLocationPermissions
	*/
	[[nodiscard]] static bool isLocationAvailable();

	/** @returns the current location of the platform's GPS.
	    The instance returned might be null if this feature is not available
	    due to platform limitations or lacking platform permissions.
	    @see requestLocationPermissions
	*/
	[[nodiscard]] static GPSLocation getCurrentLocation();

	/** @returns true if the location is null.
	    In the case of a null representation, the latitude and longitude
	    will be set to infinity.
	    The vertical axis is irrespective of the location and so the
	    altitude is not considered part of the null-ness.
	*/
	[[nodiscard]] bool isNull() const;

	GPSLocation& operator= (const GPSLocation& other);
	bool         operator== (const GPSLocation& other) const;
	bool         operator!= (const GPSLocation& other) const;

	double latitude, longitude;  //< The coordinates in degrees.
	double altitude = 0.0;       //< The altitude. Though typically this is in meters, it is platform dependant.
};

}  // namespace lemons
