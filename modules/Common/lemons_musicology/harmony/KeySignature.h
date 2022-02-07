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


namespace lemons::music
{

class KeySignature final
{
public:

	/** Creates a key signature object representing C major. */
	constexpr KeySignature() noexcept;

	constexpr explicit KeySignature (int numSharpsOrFlats, bool isSharps, bool isMajorKey) noexcept;

	constexpr explicit KeySignature (bool isMajorKey, bool isSharps, int pitchClassOfRoot) noexcept;

	[[nodiscard]] static KeySignature fromStringDescription (const String& description);

	constexpr KeySignature (const KeySignature& other) noexcept;

	constexpr bool operator== (const KeySignature& other) const noexcept;

	constexpr bool operator!= (const KeySignature& other) const noexcept;

	[[nodiscard]] constexpr KeySignature getRelativeKey() const noexcept;

	[[nodiscard]] constexpr bool isRelativeKeyOf (const KeySignature& other) const noexcept;

	[[nodiscard]] constexpr KeySignature getParallelKey() const noexcept;

	[[nodiscard]] constexpr bool isParallelKeyOf (const KeySignature& other) const noexcept;

	[[nodiscard]] constexpr bool hasEnharmonicKey() const noexcept;

	[[nodiscard]] constexpr KeySignature getEnharmonicKey() const noexcept;

	[[nodiscard]] constexpr bool isEnharmonicKeyOf (const KeySignature& other) const noexcept;

	[[nodiscard]] constexpr KeySignature getDominantKey() const noexcept;

	[[nodiscard]] constexpr bool isDominantKeyOf (const KeySignature& other) const noexcept;

	[[nodiscard]] constexpr bool isFlatKey() const noexcept;

	[[nodiscard]] constexpr bool isSharpKey() const noexcept;

	[[nodiscard]] constexpr bool isMajorKey() const noexcept;

	[[nodiscard]] constexpr bool isMinorKey() const noexcept;

	[[nodiscard]] constexpr int getNumSharps() const noexcept;

	[[nodiscard]] constexpr int getNumFlats() const noexcept;

	[[nodiscard]] constexpr int getPitchClassOfRoot() const noexcept;

	[[nodiscard]] String getRootAsString() const noexcept;

	[[nodiscard]] String getStringDescription() const noexcept;

	[[nodiscard]] constexpr int getPitchClassOfScaleDegree (int scaleDegree) const noexcept;

	[[nodiscard]] String getScaleDegreeAsString (int scaleDegree) const noexcept;

	[[nodiscard]] bool containsPitchClass (int pitchClass) const;

	[[nodiscard]] juce::Array<int> getIntervals() const;

private:

	int numAccidentals { 0 };

	bool isFlat { false }, isMajor { true };
};

}  // namespace lemons::music
