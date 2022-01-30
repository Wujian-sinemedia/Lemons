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

namespace lemons::gui::components
{

VersionViewer::VersionViewer (const Version& versionToUse)
	: version (versionToUse)
{
	util::addAndMakeVisible (*this, major, minor, patch);
}

void VersionViewer::resized()
{
	// to do
}


ToggleableVersionViewer::ToggleableVersionViewer()
{
	addAndMakeVisible (viewer);
}

void ToggleableVersionViewer::showProjectVersion()
{
	viewer.create<VersionViewer> (projectVersion);
}

void ToggleableVersionViewer::showJuceVersion()
{
	viewer.create<VersionViewer> (juceVersion);
}

void ToggleableVersionViewer::showLemonsVersion()
{
	viewer.create<VersionViewer> (lemonsVersion);
}

}  // namespace lemons::gui::components
