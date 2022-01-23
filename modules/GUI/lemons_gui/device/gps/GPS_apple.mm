#if !(JUCE_MAC || JUCE_IOS)
#error
#endif

#if !LEMONS_ENABLE_GPS_LOCATION_SERVICES
#error
#endif

#include <AppKit/AppKit.h>
#include <CoreLocation/CoreLocation.h>

namespace lemons {

bool GPSLocation::requestLocationPermissions(bool) {
#if JUCE_IOS && (defined(__IPHONE_7_0) &&                                      \
                 __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_7_0)
  if (!isLocationAvailable()) {
    id<NSApplicationDelegate> appDelegate =
        [[NSApplication sharedApplication] delegate];

    if (auto *locationManager = [appDelegate locationManager])
      [locationManager requestWhenInUseAuthorization];
  }
#endif

  return isLocationAvailable();
}

bool GPSLocation::isLocationAvailable() {
  return
      [CLLocationManager locationServicesEnabled] &&
      [CLLocationManager authorizationStatus] !=
          kCLAuthorizationStatusNotDetermined &&
      [CLLocationManager authorizationStatus] != kCLAuthorizationStatusDenied &&
      [CLLocationManager authorizationStatus] !=
          kCLAuthorizationStatusRestricted;
}

GPSLocation GPSLocation::getCurrentLocation() {
  // If you reach this, you need to prompt the user to enable using the location
  // services!
  jassert([CLLocationManager authorizationStatus] !=
          kCLAuthorizationStatusNotDetermined);

  id<NSApplicationDelegate> appDelegate =
      [[NSApplication sharedApplication] delegate];

  if (auto *locationManager = [appDelegate locationManager])
    locationManager.startUpdatingLocation();

  if (isLocationAvailable()) {
    id<NSApplicationDelegate> appDelegate =
        [[NSApplication sharedApplication] delegate];
    if (auto *locationManager = [appDelegate getLocationManager]) {
      return GPSLocation{
          static_cast<double>(locationManager.location.coordinate.latitude),
          static_cast<double>(locationManager.location.coordinate.longitude),
          static_cast<double>(locationManager.location.altitude)};
    }
  }

  return {};
}

}
