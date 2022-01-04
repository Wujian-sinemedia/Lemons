namespace lemons
{

GPSLocation::GPSLocation()
    : latitude (std::numeric_limits<double>::infinity())
    , longitude (std::numeric_limits<double>::infinity())
{
}

GPSLocation::GPSLocation (double lat, double lo, double a)
    : latitude (lat)
    , longitude (lo)
    , altitude (a)
{
}

GPSLocation::GPSLocation (const GPSLocation& other)
    : GPSLocation()
{
	operator= (other);
}

GPSLocation& GPSLocation::operator= (const GPSLocation& other)
{
	latitude  = other.latitude;
	longitude = other.longitude;
	altitude  = other.altitude;
	return *this;
}

bool GPSLocation::operator== (const GPSLocation& other) const
{
	return latitude == other.latitude
	    && longitude == other.longitude
	    && altitude == other.altitude;
}

bool GPSLocation::operator!= (const GPSLocation& other) const
{
	return ! operator== (other);
}

bool GPSLocation::isNull() const
{
	return latitude == std::numeric_limits<double>::infinity()
	    && longitude == std::numeric_limits<double>::infinity();
}

}  // namespace lemons
