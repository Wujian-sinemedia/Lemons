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

	[[nodiscard]] constexpr int getPitchClassOfScaleDegree (int scaleDegree) const noexcept;

	[[nodiscard]] String getScaleDegreeAsString (int scaleDegree) const noexcept;

	[[nodiscard]] String getStringDescription() const final;

	[[nodiscard]] int getPitchClassOfRoot() const noexcept final;

	[[nodiscard]] int getNumSharps() const noexcept final;

	[[nodiscard]] int getNumFlats() const noexcept final;

	[[nodiscard]] juce::Array<int> getIntervalsAsSemitones() const final;

	[[nodiscard]] int notesPerOctave() const noexcept final;

	[[nodiscard]] String getRootAsString() const noexcept final;

private:

	[[nodiscard]] static constexpr bool useSharpsForRootByDefault (int root) noexcept;

	[[nodiscard]] static constexpr bool rootHasEnharmonicKey (int root) noexcept;

	int numAccidentals { 0 };

	bool isFlat { false };

	Type type { Type::Major };
};

}  // namespace lemons::music
