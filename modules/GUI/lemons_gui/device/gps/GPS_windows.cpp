/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

#if ! JUCE_WINDOWS
#	error
#endif

#if ! LEMONS_ENABLE_GPS_LOCATION_SERVICES
#	error
#endif

#pragma comment(lib, "locationapi.lib")

namespace lemons
{

bool GPSLocation::requestLocationPermissions (bool coarseLocation)
{
	ComSmartPtr<ILocation> ilocation;
	if (SUCCEEDED (ilocation.CoCreateInstance (CLSID_Location)))
	{
		IID types[] = { coarseLocation ? IID_IDefaultLocation : IID_ILatLongReport };
		return SUCCEEDED (ilocation->RequestPermissions (nullptr, types, (ULONG) 1, TRUE));
	}

	return false;
}

bool GPSLocation::isLocationAvailable()
{
	ComSmartPtr<ILocation> ilocation;
	if (SUCCEEDED (ilocation.CoCreateInstance (CLSID_Location)))
	{
		auto status = REPORT_NOT_SUPPORTED;
		return SUCCEEDED (ilocation->GetReportStatus (IID_ILatLongReport, &status)) && status == REPORT_RUNNING;
	}

	return false;
}

GPSLocation GPSLocation::getCurrentLocation()
{
	ComSmartPtr<ILocation> ilocation;
	if (FAILED (ilocation.CoCreateInstance (CLSID_Location)))
	{
		jassertfalse;
		return {};
	}

	ComSmartPtr<ILatLongReport> latLongReport;
	double						latitude = 0.0, longitude = 0.0, altitude = 0.0;

	auto parseReport = [&]() -> bool
	{
		return latLongReport != nullptr
			&& SUCCEEDED (latLongReport->GetLatitude (&latitude))
			&& SUCCEEDED (latLongReport->GetLongitude (&longitude))
			&& SUCCEEDED (latLongReport->GetAltitude (&altitude));
	};

	// Attempt getting the most up to date location:
	ComSmartPtr<ILocationReport> locationReport;

	if (SUCCEEDED (ilocation->GetReport (IID_ILatLongReport, locationReport.resetAndGetPointerAddress()))
		&& SUCCEEDED (locationReport->QueryInterface (latLongReport.resetAndGetPointerAddress()))
		&& parseReport())
	{
		return GPSLocation (latitude, longitude, altitude);
	}

	// Attempt getting the default location:
	ComSmartPtr<IDefaultLocation> defaultLocation;

	if (SUCCEEDED (defaultLocation.CoCreateInstance (CLSID_DefaultLocation))
		&& SUCCEEDED (defaultLocation->QueryInterface (latLongReport.resetAndGetPointerAddress()))
		&& parseReport())
	{
		return GPSLocation (latitude, longitude, altitude);
	}

	return {};
}

}  // namespace lemons
