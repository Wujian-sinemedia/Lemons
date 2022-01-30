
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

namespace lemons::gui::components
{

VersionBumpPrompt::VersionBumpPrompt (Version& versionToUse, std::function<void()> toClose)
	: PopupComponentBase (toClose), version (versionToUse)
{
	util::addAndMakeVisible (*this, mainText, viewerLabel, viewer, bumpPatch, bumpMinor, bumpMajor);
}

VersionBumpPrompt::VersionBumpPrompt (Version& versionToUse, std::unique_ptr<PopupComponentBase>& holder)
	: PopupComponentBase (holder), version (versionToUse)
{
	util::addAndMakeVisible (*this, mainText, viewerLabel, viewer, bumpPatch, bumpMinor, bumpMajor);
}

void VersionBumpPrompt::resizeTriggered()
{
	// position buttons, viewer, labels
}

}  // namespace lemons::gui::components
