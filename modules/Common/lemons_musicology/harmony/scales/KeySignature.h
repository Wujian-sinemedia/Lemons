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


namespace lemons::music::scales
{

class KeySignature final : public Scale
{
public:

	enum class Type
	{
		Major,
		NaturalMinor,
		HarmonicMinor
		// MelodicMinor
	};

	/** Creates a key signature object representing C major. */
	constexpr KeySignature() noexcept;

	constexpr explicit KeySignature (int numSharpsOrFlats, bool isSharps, Type typeToUse) noexcept;

	constexpr explicit KeySignature (Type typeToUse, bool isSharps, int pitchClassOfRoot) noexcept;

	constexpr explicit KeySignature (Type typeToUse, int pitchClassOfRoot) noexcept;

	[[nodiscard]] static KeySignature fromStringDescription (const String& description);

	constexpr KeySignature (const KeySignature& other) noexcept;

	[[nodiscard]] constexpr bool operator== (const KeySignature& other) const noexcept;

	[[nodiscard]] constexpr bool operator!= (const KeySignature& other) const noexcept;

	[[nodiscard]] KeySignature getRelativeKey() const noexcept;

	[[nodiscard]] constexpr bool isRelativeKeyOf (const KeySignature& other) const noexcept;

	[[nodiscard]] KeySignature getParallelKey() const noexcept;

	[[nodiscard]] bool isParallelKeyOf (const KeySignature& other) const noexcept;

	[[nodiscard]] bool hasEnharmonicKey() const noexcept;

	[[nodiscard]] KeySignature getEnharmonicKey() const noexcept;

	[[nodiscard]] bool isEnharmonicKeyOf (const KeySignature& other) const noexcept;

	[[nodiscard]] KeySignature getDominantKey() const noexcept;

	[[nodiscard]] bool isDominantKeyOf (const KeySignature& other) const noexcept;

	[[nodiscard]] constexpr bool isFlatKey() const noexcept;

	[[nodiscard]] constexpr bool isSharpKey() const noexcept;

	[[nodiscard]] constexpr bool isMajorKey() const noexcept;

	[[nodiscard]] constexpr bool isMinorKey() const noexcept;

	[[nodiscard]] String getScaleDegreeAsString (int scaleDegree) const noexcept final;

	[[nodiscard]] String getStringDescription() const final;

	[[nodiscard]] int getPitchClassOfRoot() const noexcept final;

	[[nodiscard]] constexpr int getNumSharps() const noexcept;

	[[nodiscard]] constexpr int getNumFlats() const noexcept;

	[[nodiscard]] juce::Array<int> getIntervalsAsSemitones() const final;

	[[nodiscard]] int notesPerOctave() const noexcept final;

	[[nodiscard]] String getRootAsString() const noexcept final;

private:

	[[nodiscard]] static constexpr bool useSharpsForRootByDefault (int root) noexcept;

	[[nodiscard]] static constexpr bool rootHasEnharmonicKey (int root) noexcept;

	[[nodiscard]] constexpr bool hasOppositeTonality (const KeySignature& other) const noexcept;

	int numAccidentals { 0 };

	const bool isFlat { false };

	const Type type { Type::Major };
};

}  // namespace lemons::music::scales
