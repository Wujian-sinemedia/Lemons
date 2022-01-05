#pragma once

namespace lemons::gui::components
{

class VersionBumpPrompt : public PopupComponentBase
{
public:

	explicit VersionBumpPrompt (Version& versionToUse, std::function<void()> toClose);

	explicit VersionBumpPrompt (Version& versionToUse, std::unique_ptr<PopupComponentBase>& holder);


private:

	void resizeTriggered() final;

	Version& version;

	Label mainText { "Would you like to bump your project version?" };

	Label viewerLabel { "Current version:" };

	VersionViewer viewer { version };

	TextButton bumpPatch { TRANS ("Bump patch"), [&]()
		                   { this->version.bumpPatch(); this->close(); } };
	TextButton bumpMinor { TRANS ("Bump minor"), [&]()
		                   { this->version.bumpMinor(); this->close(); } };
	TextButton bumpMajor { TRANS ("Bump major"), [&]()
		                   { this->version.bumpMajor(); this->close(); } };
};

}  // namespace lemons::gui::components
