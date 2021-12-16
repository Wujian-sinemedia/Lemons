namespace juce
{

RSAKey VariantConverter<RSAKey>::fromVar (const var& v)
{
    RSAKey key { v.toString() };
    return key;
}

var VariantConverter<RSAKey>::toVar (const RSAKey& k)
{
    return { k.toString() };
}

}

/*------------------------------------------------------------------------------------------------------------------*/

namespace lemons::crypto
{

String encryptString (const String& stringToEncrypt, const RSAKey& publicKey)
{
    juce::CharPointer_UTF8 stringPointer (stringToEncrypt.toUTF8());
    juce::MemoryBlock stringMemoryBlock (stringPointer.getAddress(), stringPointer.sizeInBytes());
    
    juce::BigInteger stringAsData;
    stringAsData.loadFromMemoryBlock (stringMemoryBlock);
    
    publicKey.applyToValue (stringAsData);
    
    return stringAsData.toMemoryBlock().toBase64Encoding();
}

String decryptString (const String& encryptedString, const RSAKey& privateKey)
{
    juce::MemoryBlock encryptedMemoryBlock;
    
    encryptedMemoryBlock.fromBase64Encoding (encryptedString);
    
    juce::BigInteger stringAsData;
    stringAsData.loadFromMemoryBlock (encryptedMemoryBlock);
    
    privateKey.applyToValue (stringAsData);
    
    return stringAsData.toMemoryBlock().toString();
}


void generateKeyPair (const juce::File& outputFile, int numBits, const juce::Array<int>& randomSeeds)
{
    outputFile.deleteFile();
    
    RSAKey publicKey, privateKey;
    
    if (randomSeeds.isEmpty())
    {
        RSAKey::createKeyPair (publicKey, privateKey, numBits);
    }
    else
    {
        if (randomSeeds.size() < 3)
            DBG ("Warning - you should provide more than 2 random seed values!");
        
        RSAKey::createKeyPair (publicKey, privateKey, numBits, randomSeeds.getRawDataPointer(), randomSeeds.size());
    }
    
    String fileText;
    
    fileText << "Public key: " << publicKey.toString() << juce::newLine << "Private key: " << privateKey.toString();
    
    outputFile.replaceWithText (fileText);
}

}

