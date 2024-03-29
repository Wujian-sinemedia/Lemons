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

#include <juce_cryptography/juce_cryptography.h>

namespace juce
{

template <>
struct VariantConverter<RSAKey>
{
	static RSAKey fromVar (const var& v);
	static var	  toVar (const RSAKey& k);
};

}  // namespace juce

namespace lemons::rsa
{

using juce::RSAKey;
using juce::String;


[[nodiscard]] String encryptString (const String& stringToEncrypt, const RSAKey& publicKey);

[[nodiscard]] String decryptString (const String& encryptedString, const RSAKey& privateKey);


template <typename T>
[[nodiscard]] String encryptObject (const T& object, const RSAKey& publicKey)
{
	const auto var = juce::VariantConverter<T>::toVar (object);

	return encryptString (var.toString(), publicKey);
}

template <typename T>
[[nodiscard]] T decryptObject (const String& encryptedString, const RSAKey& privateKey)
{
	const juce::var var { decryptString (encryptedString, privateKey) };

	return juce::VariantConverter<T>::fromVar (var);
}


// generates a pair of RSA keys and writes them to a file
void generateKeyPair (const juce::File&		  outputFile,
					  int					  numBits	  = 128,
					  const juce::Array<int>& randomSeeds = {});

}  // namespace lemons::rsa
