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

#pragma once

namespace lemons::gui::components
{

class VersionViewer : public Component
{
public:

	explicit VersionViewer (const Version& versionToUse);

private:

	void resized() final;

	const Version& version;

	Label major { String (version.getMajor()) };
	Label minor { String (version.getMinor()) };
	Label patch { String (version.getPatch()) };
};


class ToggleableVersionViewer : public Component
{
public:

	ToggleableVersionViewer();

	void showProjectVersion();

	void showJuceVersion();

	void showLemonsVersion();

	/*
	 to do: drop down menu for toggling?
	 */

private:

	SwappableComponent viewer;

	static constexpr auto projectVersion = Version::projectVersion();
	static constexpr auto juceVersion	 = Version::juceVersion();
	static constexpr auto lemonsVersion	 = Version::lemonsVersion();
};

}  // namespace lemons::gui::components
