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

#pragma once

namespace lemons::midi
{

/** @ingroup lemons_midi
 */
struct VelocityHelper
{
	void setSensitivity (float newSensitivity);
	void setSensitivity (int newSensitivity);

	float getGainForVelocity (float midiVelocity);
	float getGainForVelocity (int midiVelocity);

	float getSensitivity() const { return sensitivity; }

private:

	float sensitivity { 1.0f };
};

}  // namespace lemons::midi
