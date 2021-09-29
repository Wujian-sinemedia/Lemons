
#pragma once

namespace lemons
{
extern void initializeTranslations (const File& translationFile,
                                    bool        ignoreCaseOfKeys = true);


extern void initializeTranslationsFromBinaryData (RawData data,
                                                  bool    ignoreCaseOfKeys = true);


extern void initializeDefaultTranslations();


}  // namespace lemons
