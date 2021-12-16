namespace lemons
{

OnlineUnlock::OnlineUnlock (const String& productID, const String& websiteName, const String& authURL, const String& publicKey, SavingFunc&& funcToSave, LoadingFunc&& funcToLoad)
    : productName (productID)
    , siteName (websiteName)
    , public_key (publicKey)
    , auth_url (authURL)
    , saveFunc (std::move (funcToSave))
    , loadFunc (std::move (funcToLoad))
{
}

String OnlineUnlock::readReplyFromWebserver (const String& email, const String& password)
{
	const juce::URL url (auth_url
	                   .withParameter ("product", getProductID())
	                   .withParameter ("email", email)
	                   .withParameter ("password", password)
	                   .withParameter ("machine", getLocalMachineIDs()[0]));

	DBG ("Authentication URL: " << url.toString (true));

	{
		juce::ScopedLock lock (streamCreationLock);
		stream.reset (new juce::WebInputStream (url, true));
	}

	if (stream->connect (nullptr))
	{
		const auto*              thread = juce::Thread::getCurrentThread();
		juce::MemoryOutputStream result;

		while (! (stream->isExhausted() || stream->isError()
		          || (thread != nullptr && thread->threadShouldExit())))
		{
			const auto bytesRead = result.writeFromInputStream (*stream, 8192);

			if (bytesRead < 0)
				break;
		}

		return result.toString();
	}

	return {};
}

void OnlineUnlock::userCancelled()
{
	juce::ScopedLock lock (streamCreationLock);

	if (stream != nullptr)
		stream->cancel();
}

bool OnlineUnlock::hasValidLicense() const
{
	if ((bool) isUnlocked())
		return true;

	const auto ms = getExpiryTime().toMilliseconds();

	if (ms == 0)
		return false;

	return ms < juce::Time::getCurrentTime().toMilliseconds();
}

String OnlineUnlock::getProductID()
{
	return productName;
}

bool OnlineUnlock::doesProductIDMatch (const String& returned)
{
	return returned == productName;
}

juce::RSAKey OnlineUnlock::getPublicKey()
{
	return public_key;
}

String OnlineUnlock::getWebsiteName()
{
	return siteName;
}

juce::URL OnlineUnlock::getServerAuthenticationURL()
{
	return auth_url;
}

void OnlineUnlock::saveState (const String& str)
{
	saveFunc (str);
}

String OnlineUnlock::getState()
{
	return loadFunc();
}

}  // namespace lemons
