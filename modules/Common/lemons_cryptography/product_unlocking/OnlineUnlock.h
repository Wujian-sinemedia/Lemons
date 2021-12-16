#pragma once

#include <juce_product_unlocking/juce_product_unlocking.h>


namespace lemons
{

using juce::String;


class OnlineUnlock final : public juce::OnlineUnlockStatus
{
public:
	using SavingFunc  = std::function<void (const String&)>;
	using LoadingFunc = std::function<String()>;

	explicit OnlineUnlock (const String& productID, const String& websiteName, const String& authURL, const String& publicKey,
	                       SavingFunc&& funcToSave, LoadingFunc&& funcToLoad);

	bool hasValidLicense() const;

private:
	String getProductID() final;

	juce::RSAKey getPublicKey() final;

	String getWebsiteName() final;

	bool doesProductIDMatch (const String& returned) final;

	juce::URL getServerAuthenticationURL() final;

	String readReplyFromWebserver (const String& email, const String& password) final;

	void userCancelled() final;

	void saveState (const String& str) final;

	String getState() final;

	String productName, siteName;

	juce::RSAKey public_key;

	juce::URL auth_url;

	SavingFunc  saveFunc;
	LoadingFunc loadFunc;

	juce::CriticalSection                 streamCreationLock;
	std::unique_ptr<juce::WebInputStream> stream;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnlineUnlock)
};

}  // namespace lemons
