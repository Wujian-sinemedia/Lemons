namespace lemons
{

constexpr Dimensions::Dimensions (int widthToUse, int heightToUse)
    : width (widthToUse)
    , height (heightToUse)
{
}

constexpr bool Dimensions::isValid() const noexcept
{
	return width > 0 && height > 0;
}

constexpr float Dimensions::getAspectRatio() const noexcept
{
	if (! isValid())
		return 0.f;

	return static_cast<float> (width) / static_cast<float> (height);
}

constexpr Dimensions Dimensions::getDefault()
{
	return { 1060, 640 };
}

}  // namespace lemons
