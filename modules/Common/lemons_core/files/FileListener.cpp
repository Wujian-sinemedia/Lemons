namespace lemons::files
{

Listener::Listener (const File& fileToWatch,
	std::function<void()>&&		callback)
	: file (fileToWatch), callbackFunc (callback)
{
	lastModTime = file.getLastModificationTime().toMilliseconds();
}

void Listener::check()
{
	const auto newModTime = file.getLastModificationTime().toMilliseconds();

	if (newModTime != lastModTime)
	{
		lastModTime = newModTime;
		callbackFunc();
	}
}

}  // namespace lemons::files
